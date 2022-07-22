#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sstream>

class Transform
{
private:

    std::string name;

    // TRSA!
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 anchor = glm::vec3(0.0);

public:

    Transform() = default;
    Transform(const std::string& name): name(name) {}

    const std::string& getName() const { return name; }
    void setName(const std::string &name) { this->name = name; }

    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getRotation() const { return rotation; }
    const glm::vec3 &getScale() const { return scale; }
    const glm::vec3 &getAnchor() const { return anchor; }

    void setPosition(const glm::vec3 &position) { this->position = position; }
    void setRotation(const glm::vec3 &rotation) { this->rotation = rotation; }
    void setScale(const glm::vec3 &scale) { this->scale = scale; }
    void setAnchor(const glm::vec3 &anchor) { this->anchor = anchor; }

    glm::mat4 computeModelMatrix(const glm::mat4 &origin)
    {
        // TODO Optimize. 

        // TRANSLATE
        glm::mat4 model = glm::translate(origin, position);

        // ROTATE
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

        // SCALE
        model = glm::scale(model, scale);

        // ANCHOR
        model = glm::translate(model, -anchor);


        return model;
    }

    void renderImgui(float max)
    {
        ImGui::PushID(name.c_str());

        ImGui::SliderFloat3("position", (float*)&position[0], -max, max);
        ImGui::SliderFloat3("scale", (float*)&scale[0], -4.0f, 4.0f);
        ImGui::SliderFloat3("rotation", (float*)&rotation[0], -360, 360);
        ImGui::SliderFloat3("anchor", (float*)&anchor[0], -max, max);

        ImGui::PopID();
    }
};


#endif /*TRANSFORM_H*/