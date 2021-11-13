#include "pch.h"
#include "Material.h"

PAG::Material::Material()
{
}

PAG::Material::Material(glm::vec3 a, glm::vec3 d, glm::vec3 s, float ss): ambient(a), diffuse(d), specular(s), shininess(ss)
{
}

PAG::Material::Material(const Material& orig): ambient(orig.ambient), diffuse(orig.diffuse), specular(orig.specular), shininess(orig.shininess)
{
}

PAG::Material& PAG::Material::operator=(const Material& orig)
{
    if (this != &orig)
    {
        this->ambient = orig.ambient;
        this->diffuse = orig.diffuse;
        this->specular = orig.specular;
        this->shininess = orig.shininess;
    }
    return *this;
}

PAG::Material::~Material()
{
}
