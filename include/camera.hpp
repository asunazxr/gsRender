#pragma once

#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    UNDER
};

const float YAW = -90.0f;           
const float PITCH = 0.0f;           
const float SPEED = 5.0f;           
const float SENSITITIVITY = 0.1f;  
const float ZOOM = 45.0f;           

class Camera {
private:
    glm::vec3 OriPosition;
    glm::vec3 OriFront;
    glm::vec3 OriWorldUp;
    float OriYaw;
    float OriPitch;
    glm::mat4 perspectivem;

public:
    glm::vec3 Position;
    glm::vec2 focal = glm::vec2(1164.6601287484507,1159.5880733038064);
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSentitivity;
    float Zoom;
    float near = 1.0f;
    float far = 100.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 12.0f),
           glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH)
        : MovementSpeed(SPEED), MouseSentitivity(SENSITITIVITY), Zoom(ZOOM)
    {
        Position = position;
        OriPosition = position;
        Front = front;
        OriFront = front;
        WorldUp = up;
        OriWorldUp = WorldUp;
        Yaw = yaw;
        Pitch = pitch;
        OriYaw = yaw;
        OriPitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() const {
        return lookAt(Position, Position + Front, WorldUp);
    }

    void setPerspective(int& width, int& height) {
        float fovy_rad = glm::radians(Zoom); // 转弧度
        float aspect = (float)width / height;
        perspectivem = glm::perspective(fovy_rad, aspect, near, far);
    }

    void setPerspectiveByFocal(int& width, int& height) {
        
    }
    glm::mat4 getPerspective() const {
        return perspectivem;
    }

    void reset() {
        Position = OriPosition;
        Front = OriFront;
        WorldUp = OriWorldUp;
        Yaw = OriYaw;
        Pitch = OriPitch;
        updateCameraVectors();
    }

    void ProcessKeyBoard(Camera_Movement direction, float deltaTime) {
        float velocity = deltaTime * MovementSpeed;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == UP)
            Position += Up * velocity;
        if (direction == UNDER)
            Position -= Up * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool controlPitch = true) {
        xoffset *= MouseSentitivity;
        yoffset *= MouseSentitivity;
        Yaw += xoffset;
        Pitch += yoffset;
        if (controlPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 90.0f)
            Zoom = 90.0f;
    }

    glm::mat4 myLookAtViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        glm::vec3 camZ = glm::normalize(position - target);
        glm::vec3 camX = glm::normalize(cross(up, camZ));
        glm::vec3 camY = glm::normalize(cross(camZ, camX));
        glm::mat4 translate(1.0f);
        translate[3][0] = -position.x;
        translate[3][1] = -position.y;
        translate[3][2] = -position.z;
        glm::mat4 rotate(1.0f);
        rotate[0][0] = camX.x;
        rotate[1][0] = camX.y;
        rotate[2][0] = camX.z;
        rotate[0][1] = camY.x;
        rotate[1][1] = camY.y;
        rotate[2][1] = camY.z;
        rotate[0][2] = camZ.x;
        rotate[1][2] = camZ.y;
        rotate[2][2] = camZ.z;
        return rotate * translate;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        front.y = sin(glm::radians(Pitch));
        front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};