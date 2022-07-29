#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

#include "ShaderProgram.h"

class Material
{
protected:
    std::shared_ptr<ShaderProgram> program;

public:

    const std::shared_ptr<ShaderProgram> &getProgram() const { return program; }
    void setProgram(const std::shared_ptr<ShaderProgram> &program) { this->program = program; }

};


#endif /* MATERIAL_H */