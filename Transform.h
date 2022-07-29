#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <sstream>

class Transform
{
private:
    // TRSA!
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 rotation = glm::vec3(0.0);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 anchor = glm::vec3(0.0);

    glm::mat4 localModelMat;
    glm::mat4 worldModelMat;

    bool dirty = true;

public:

    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getRotation() const { return rotation; }
    const glm::vec3 &getScale() const { return scale; }
    const glm::vec3 &getAnchor() const { return anchor; }

    void setPosition(const glm::vec3 &position) { this->position = position; dirty = true; }
    void setRotation(const glm::vec3 &rotation) { this->rotation = rotation; dirty = true; }
    void setScale(const glm::vec3 &scale) { this->scale = scale; dirty = true; }
    void setAnchor(const glm::vec3 &anchor) { this->anchor = anchor; dirty = true; }

    const glm::mat4 &getLocalModelMat() const { return localModelMat; }

    const glm::mat4 &getWorldModelMat() const { return worldModelMat; }

    bool isDirty() const { return dirty; }
    void setDirty(bool dirty) { this->dirty = dirty; }

    const glm::mat4 &computeModelMatrix(const glm::mat4 &origin)
    {
        if (dirty)
        {
            // TODO Optimize.

            // TRANSLATE
            localModelMat = glm::translate(glm::mat4(1.0f), position);

            // ROTATE
            localModelMat *= glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

            // SCALE
            localModelMat = glm::scale(localModelMat, scale);

            // ANCHOR
            localModelMat = glm::translate(localModelMat, -anchor);

            worldModelMat = origin * localModelMat;

            //dirty = false;
        }

        return worldModelMat;
    }

    void renderImgui(float max)
    {
        char buf[32];

        ImGui::SliderFloat3("position", (float*)&position[0], -max, max);
        ImGui::SliderFloat3("scale", (float*)&scale[0], -4.0f, 4.0f);
        ImGui::SliderFloat3("rotation", (float*)&rotation[0], -360, 360);
        ImGui::SliderFloat3("anchor", (float*)&anchor[0], -max, max);

        if (ImGui::BeginTable("Model (local)", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    snprintf(buf, sizeof(buf), "%3.2f", localModelMat[row][column]);
                    ImGui::TextUnformatted(buf);
                }
            }
            ImGui::EndTable();
        }
        if (ImGui::BeginTable("Model (world)", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    snprintf(buf, sizeof(buf), "%3.2f", worldModelMat[row][column]);
                    ImGui::TextUnformatted(buf);
                }
            }
            ImGui::EndTable();
        }
    }
};


#endif /*TRANSFORM_H*/