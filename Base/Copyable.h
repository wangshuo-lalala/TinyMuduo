#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

namespace jing
{
class Copyable
{
protected:
    Copyable() = default;
    ~Copyable() = default;
};
}
#endif