#ifndef SCENENODE_H
#define SCENENODE_H

#include <memory>
#include <vector>

#include "Transform.h"

class SceneNode : std::enable_shared_from_this<SceneNode>
{
protected:

    std::string name;
    Transform transform;

    std::weak_ptr<SceneNode> parent;
    std::vector<std::shared_ptr<SceneNode>> children;

    SceneNode() {};
    SceneNode(const std::string &name) : name(name), transform(name) {};

public:

    template<typename... TArgs>
    static std::shared_ptr<SceneNode> Create(TArgs&... args)
    {
        return std::shared_ptr<SceneNode>(new SceneNode(args...));
    }

    std::shared_ptr<SceneNode> getParent() const;
    void setParent(const std::shared_ptr<SceneNode> parent);

    size_t getChildCount() const;
    std::shared_ptr<SceneNode> getChild(size_t index) const;
    std::shared_ptr<SceneNode> findChild(const std::string &name) const;
    bool addChild(const std::shared_ptr<SceneNode>& child);
    bool addChildAt(const std::shared_ptr<SceneNode>& child, size_t index);
    bool removeChild(const std::shared_ptr<SceneNode>& child);
    bool removeChildAt(size_t index);
};

inline std::shared_ptr<SceneNode> SceneNode::getParent() const
{
    return parent.lock();
}

inline void SceneNode::setParent(const std::shared_ptr<SceneNode> parent)
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

size_t SceneNode::getChildCount() const
{
    return children.size();
}

std::shared_ptr<SceneNode> SceneNode::findChild(const std::string &name) const
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

std::shared_ptr<SceneNode> SceneNode::getChild(size_t index) const
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


#endif /* SCENENODE_H */
