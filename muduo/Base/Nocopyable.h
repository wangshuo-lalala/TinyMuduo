#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H
namespace jing
{
class Nocopyable
{
public:
    Nocopyable(const Nocopyable& rhs) = delete;
    Nocopyable& operator=(const Nocopyable& rhs) = delete;

protected:
    Nocopyable() = default;
    ~Nocopyable() = default;
};
}
#endif