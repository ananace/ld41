#include "Math.hpp"

template<typename T>
sf::Vector2<T> Math::ceil(const sf::Vector2<T>& inp)
{
    return sf::Vector2<T>(std::ceil(inp.x), std::ceil(inp.y));
}
template<typename T>
sf::Vector2<T> Math::floor(const sf::Vector2<T>& inp)
{
    return sf::Vector2<T>(std::floor(inp.x), std::floor(inp.y));
}

template<typename T>
T Math::dot(const sf::Vector2<T>& first, const sf::Vector2<T>& second)
{
    return first.x * second.x + first.y * second.y;
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

template<typename T>
T Math::distance(const sf::Vector2<T>& inp1, const sf::Vector2<T>& inp2)
{
    float dX = inp2.x - inp1.x, dY = inp2.y - inp1.y;
    return std::sqrt(dX * dX + dY * dY);
}

template<typename T>
sf::Vector2<T> Math::constrain(const sf::Rect<T>& rect, const sf::Vector2<T>& point)
{
    return sf::Vector2<T>(std::max(rect.left, std::min(rect.left + rect.width, point.x)), std::max(rect.top, std::min(rect.top + rect.height, point.y)));
}
