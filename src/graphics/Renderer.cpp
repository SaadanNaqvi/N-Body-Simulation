#include "Renderer.h"



static glm::vec3 colorFromMass(double m, double mn, double mx){
    double denom = (mx > mn) ? (mx - mn) : 1.0;
    float t = (float)((m - mn) / denom);

    glm::vec3 c1(0.4f, 0.6f, 1.0f);
    glm::vec3 c2(1.0f, 0.9f, 0.6f);

    return (1.0f - t) * c1 + t * c2;
}





float lastX = 400, lastY = 300;
bool firstMouse = true;
Camera* globalCamera = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Renderer::Renderer(int width, int height){
    this->width = width;
    this->height = height;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    
    lastX = xpos;
    lastY = ypos;
    
    if (globalCamera)
        globalCamera->processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (globalCamera)
        globalCamera->processMouseScroll(yoffset);
}

void Renderer::run(System& system){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "N Body Simulation", NULL, NULL);
    if(window == nullptr){
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    double lastFPSTime = glfwGetTime();
    int frameCount = 0;

    // Capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return;
    }

    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    const char* vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    )";

    const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec3 uColor;

    void main() {
        FragColor = vec4(uColor, 1.0);
    }
    )";

    const char* trailVertexSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    layout(location = 1) in float aT; // 0..1 along trail

    uniform mat4 view;
    uniform mat4 projection;

    out float t;

    void main(){
        t = aT;
        gl_Position = projection * view * vec4(aPos, 1.0);
    }
    )";

    const char* trailFragmentSrc = R"(
    #version 330 core
    in float t;
    out vec4 FragColor;

    uniform vec3 uTrailColor;

    void main(){
        float alpha = t;
        FragColor = vec4(uTrailColor, alpha);
    }
    )";


    const char* bgVS = R"(
    #version 330 core
    const vec2 verts[3] = vec2[3](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );
    out vec2 uv;
    void main(){
        gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);
        uv = (gl_Position.xy * 0.5) + 0.5;
    }
    )";

    const char* bgFS = R"(
    #version 330 core
    in vec2 uv;
    out vec4 FragColor;

    void main(){
        // vertical gradient + subtle vignette
        vec3 top = vec3(0.02, 0.02, 0.06);
        vec3 bot = vec3(0.00, 0.00, 0.02);
        vec3 col = mix(bot, top, uv.y);

        float v = smoothstep(0.9, 0.2, length(uv - vec2(0.5)));
        col *= v;

        FragColor = vec4(col, 1.0);
    }
    )";




    Shader shader(vertexShaderSrc, fragmentShaderSrc);
    Shader trailShader(trailVertexSrc, trailFragmentSrc);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    initSphere();
    initTrails();
    updateTrails(system.getParticles());
    
    // Create camera
    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
    globalCamera = &camera;
    
    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    Shader bgShader(bgVS, bgFS);
    // Render loop
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window, camera, deltaTime, system);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glDisable(GL_DEPTH_TEST);
        bgShader.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glEnable(GL_DEPTH_TEST);

        shader.use();
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix((float)width / (float)height);
        
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
    

        drawSphere(system.getParticles(), shader);

        glDepthMask(GL_FALSE);
        drawTrails(system.getParticles(), trailShader, view, projection);
        glDepthMask(GL_TRUE);
        system.update(100000);
        updateTrails(system.getParticles());

        frameCount++;
        double currentTime = glfwGetTime();

        if (currentTime - lastFPSTime >= 1.0) {
            double fps = frameCount / (currentTime - lastFPSTime);

            std::string title = "N Body Simulation | FPS: " + std::to_string((int)fps);
            glfwSetWindowTitle(window, title.c_str());

            frameCount = 0;
            lastFPSTime = currentTime;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
}

void Renderer::processInput(GLFWwindow* window, Camera& camera, float deltaTime, System& system)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.processKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.processKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        clearTrails();
        system.randomSpawn();
    }
}

void Renderer::generateSphere(double radius, int sectors, int stackCount, std::vector<Vector3>& vertices, std::vector<unsigned int>& indices){
    for(int i = 0; i<= stackCount; i++){
        double stackAngle = PI / 2 - i * PI / stackCount;
        double xy = radius * cosf(stackAngle);
        double z = radius * sinf(stackAngle);

        for(int j = 0; j <= sectors; ++j) {
            double sectorAngle = j * 2 * PI / sectors;
            Vector3 v(xy * cosf(sectorAngle), xy * sinf(sectorAngle), z);

            vertices.push_back(v);
        }
    }

    for (int i = 0; i < stackCount; i++) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; j++, k1++, k2++) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Renderer::initSphere(){
    std::vector<Vector3> vertices;
    std::vector<unsigned int> indices;

    generateSphere(1.0, 36, 18, vertices, indices);
    indexCount = indices.size();

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    std::vector<float> verticesFloat;
    verticesFloat.reserve(vertices.size() * 3);
    for (auto& v : vertices) {
        verticesFloat.push_back((float)v.getX());
        verticesFloat.push_back((float)v.getY());
        verticesFloat.push_back((float)v.getZ());
    }

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, verticesFloat.size() * sizeof(float), verticesFloat.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Renderer::drawSphere(std::vector<Particle*>& particles, Shader& shader) {
    glBindVertexArray(sphereVAO);

    double mn = 1e300, mx = 0;
    for (auto* p : particles){
        mn = std::min(mn, p->getMass());
        mx = std::max(mx, p->getMass());
    }
    double denom = (mx > mn) ? (mx - mn) : 1.0;

    for (auto* particle : particles) {
        Vector3 position = particle->getPosition();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(
            (float)(position.getX() * visualScale),
            (float)(position.getY() * visualScale),
            (float)(position.getZ() * visualScale)
        ));

        double raw = particle->getRadius() * radiusScale;
        double clamped = std::max(minSphere, std::min(maxSphere, raw));
        model = glm::scale(model, glm::vec3((float)clamped));
        shader.setMat4("model", model);

        // map mass -> 0..1
        float t = (float)((particle->getMass() - mn) / denom);

        glm::vec3 c1(0.4f, 0.6f, 1.0f);
        glm::vec3 c2(1.0f, 0.9f, 0.6f);
        glm::vec3 color = (1.0f - t) * c1 + t * c2;

        shader.setVec3("uColor", color);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}



void Renderer::initTrails() {
    glGenVertexArrays(1, &trailVAO);
    glGenBuffers(1, &trailVBO);

    glBindVertexArray(trailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, trailVBO);


    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Renderer::updateTrails(std::vector<Particle*>& particles) {
    for (Particle* p : particles) {
        Vector3 pos = p->getPosition();

        Vector3 v(
            pos.getX() * visualScale,
            pos.getY() * visualScale,
            pos.getZ() * visualScale
        );

        auto& dq = trails[p]; dq.push_back(v);

        if (dq.size() > maxTrailPoints) dq.pop_front();
    }
}


void Renderer::drawTrails(std::vector<Particle*>& particles, Shader& trailShader,glm::mat4& view, glm::mat4& projection){
    trailShader.use();
    trailShader.setMat4("view", view);
    trailShader.setMat4("projection", projection);

    double mn = 1e300, mx = 0.0;
    for (auto* p : particles){
        mn = std::min(mn, p->getMass());
        mx = std::max(mx, p->getMass());
    }

    glBindVertexArray(trailVAO);
    glLineWidth(2.0f);

    for (Particle* p : particles){
        auto it = trails.find(p);
        if (it == trails.end()) continue;

        auto& dq = it->second;
        if (dq.size() < 2) continue;

        glm::vec3 c = colorFromMass(p->getMass(), mn, mx);
        trailShader.setVec3("uTrailColor", c);

        std::vector<float> data;
        data.reserve(dq.size() * 4);

        float nMinus1 = (dq.size() > 1) ? float(dq.size() - 1) : 1.0f;

        for (size_t i = 0; i < dq.size(); i++) {
            Vector3 v = dq[i];
            float t = float(i) / nMinus1;

            data.push_back((float)v.getX());
            data.push_back((float)v.getY());
            data.push_back((float)v.getZ());
            data.push_back(t);
        }

        glBindBuffer(GL_ARRAY_BUFFER, trailVBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);


        glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)dq.size());
    }

    glBindVertexArray(0);
}


void Renderer::clearTrails(){
    trails.clear();
}