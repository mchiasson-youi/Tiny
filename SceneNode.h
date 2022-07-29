#ifndef SCENENODE_H
#define SCENENODE_H

#include <memory>
#include <vector>

#include "Material.h"
#include "Mesh.h"
#include "Transform.h"


class SceneNode : public std::enable_shared_from_this<SceneNode>
{
protected:

    std::string name;
    Transform transform;

    std::shared_ptr<Material> material;
    std::shared_ptr<Mesh> mesh;

    std::weak_ptr<SceneNode> parent;
    std::vector<std::shared_ptr<SceneNode>> children;

    SceneNode();
    SceneNode(const std::string &name);

    SceneNode(const SceneNode&) = delete;
    SceneNode& operator=(const SceneNode&) = delete;

public:

    ~SceneNode();

    template<typename... TArgs>
    static std::shared_ptr<SceneNode> Create(TArgs&... args)
    {
        return std::shared_ptr<SceneNode>(new SceneNode(args...));
    }

    std::shared_ptr<SceneNode> getParent() const;
    void setParent(const std::shared_ptr<SceneNode> &parent);

    size_t getChildCount() const;
    std::shared_ptr<SceneNode> getChild(size_t index) const;
    std::shared_ptr<SceneNode> findChild(const std::string &name) const;
    bool addChild(const std::shared_ptr<SceneNode>& child);
    bool addChildAt(const std::shared_ptr<SceneNode>& child, size_t index);
    bool removeChild(const std::shared_ptr<SceneNode>& child);
    bool removeChildAt(size_t index);

    const std::shared_ptr<Material> &getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &material);

    const std::shared_ptr<Mesh> &getMesh() const;
    void setMesh(const std::shared_ptr<Mesh> &mesh);

    Transform& getTransform();
    const Transform& getTransform() const;
    void setTransform(const Transform &transform);

    void update(const glm::mat4 &origin = glm::mat4(1.0f));
    void render();

    void renderImgui(float max);
};


inline SceneNode::SceneNode() 
{

}

inline SceneNode::SceneNode(const std::string &name) 
    : name(name)
{

}

inline SceneNode::~SceneNode()
{
    printf("SceneNode '%s' was destroyed\n", name.c_str());
}

inline std::shared_ptr<SceneNode> SceneNode::getParent() const
{
    return parent.lock();
}

inline void SceneNode::setParent(const std::shared_ptr<SceneNode> &parent)
{
    auto thiz = shared_from_this();

    // make sure we cannot set itself as its own parent
    if (parent == thiz)
    {
        return;
    }

    auto currentParent = getParent();
    if (currentParent)
    {
        currentParent->removeChild(thiz);
    }

    if(parent)
    {
        parent->addChild(thiz);
    }
}

inline size_t SceneNode::getChildCount() const
{
    return children.size();
}

inline std::shared_ptr<SceneNode> SceneNode::findChild(const std::string &name) const
{
    if (this->name == name)
    {
        return std::const_pointer_cast<SceneNode>(shared_from_this());
    }

    for (size_t i = 0; i < children.size(); ++i)
    {
        const auto &child = children[i];
        if (child->name == name)
        {
            return child;
        }
    }

    for (size_t i = 0; i < children.size(); ++i)
    {
        auto node = children[i]->findChild(name);
        if (node)
        {
            return node;
        }
    }

    return std::shared_ptr<SceneNode>();
}

inline std::shared_ptr<SceneNode> SceneNode::getChild(size_t index) const
{
    return children[index];
}

inline bool SceneNode::addChild(const std::shared_ptr<SceneNode>& child)
{
    return addChildAt(child, children.size());
}

inline bool SceneNode::addChildAt(const std::shared_ptr<SceneNode>& child, size_t index)
{
    // avoid adding a null child.
    if (!child)
    {
        return false;
    }

    auto thiz = shared_from_this();

    // avoid adding itself as a child
    if (child == thiz)
    {
        return false;
    }

    auto currentParent = child->getParent();
    if (currentParent)
    {
        currentParent->removeChild(child);
    }

    if (index >= children.size())
    {
        children.push_back(child);
    }
    else
    {
        children.insert(children.begin() + index, child);
    }

    child->parent = thiz;

    return true;
}

inline bool SceneNode::removeChild(const std::shared_ptr<SceneNode>& child)
{
    if (!child)
    {
        return false;
    }

    for(size_t i = 0; i < children.size(); ++i)
    {
        if (child == children[i])
        {
            child->parent.reset();
            children.erase(children.begin() + i);
            return true;
        }
    }

    return false;
}

inline bool SceneNode::removeChildAt(size_t index)
{
    if (index <= (children.size() - 1))
    {
        children[index]->parent.reset();
        children.erase(children.begin() + index);
        return true;
    }
    return false;
}

inline const std::shared_ptr<Material> &SceneNode::getMaterial() const
{
    return material;
}

inline void SceneNode::setMaterial(const std::shared_ptr<Material> &material)
{
    this->material = material;
}

inline const std::shared_ptr<Mesh> &SceneNode::getMesh() const
{
    return mesh;
}

inline void SceneNode::setMesh(const std::shared_ptr<Mesh> &mesh)
{
    this->mesh = mesh;
}

inline Transform& SceneNode::getTransform()
{
    return transform;
}

inline const Transform& SceneNode::getTransform() const
{
    return transform;
}

inline void SceneNode::setTransform(const Transform &transform)
{
    this->transform = transform;
}

inline void SceneNode::update(const glm::mat4 &origin)
{
    const glm::mat4 &model = transform.computeModelMatrix(origin);

    for (size_t i = 0; i < children.size(); ++i)
    {
        children[i]->update(model);
    }
}

inline void SceneNode::render()
{
    if(material && mesh)
    {
        const auto &program = material->getProgram();
        if (program)
        {
            program->activate();
            program->setModel(transform.getWorldModelMat());

            const auto &vertexBuffer = mesh->getVertexBuffer();
            if (vertexBuffer)
            {
                vertexBuffer->activate();

                glDrawArrays(mesh->getPrimitive(), mesh->getVertexStartOffset(), mesh->getVertexCount());

                vertexBuffer->deactivate();
            }
            program->deactivate();
        }
    }

    for(size_t i = 0; i < children.size(); ++i)
    {
        children[i]->render();
    }
}

void SceneNode::renderImgui(float max)
{
    if (ImGui::TreeNode(name.c_str()))
    {
        transform.renderImgui(max);

        for (int i = 0; i < children.size(); ++i)
        {
            children[i]->renderImgui(max);
        }

        ImGui::TreePop();
    }
}

#endif /* SCENENODE_H */
