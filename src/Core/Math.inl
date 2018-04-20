#include "Math.hpp"

template<typename T>
sf::Vector2<T> Math::ceil(const sf::Vector2<T>& inp)
{
    return sf::Vector2<T>(std::ceil(inp.X), std::ceil(inp.Y));
}
template<typename T>
sf::Vector2<T> Math::floor(const sf::Vector2<T>& inp)
{
    return sf::Vector2<T>(std::floor(inp.X), std::floor(inp.Y));
}

template<typename T>
T Math::dot(const sf::Vector2<T>& first, const sf::Vector2<T>& second)
{
    return first.X * second.X + first.Y * second.Y;
}

template<typename T>
T Math::lengthSquared(const sf::Vector2<T>& inp)
{
    return dot(inp, inp);
}
template<typename T>
T Math::length(const sf::Vector2<T>& inp)
{
    return std::sqrt(lengthSquared(inp));
}
