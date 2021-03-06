#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <cmath>

namespace Math
{

template<typename T>
constexpr T PI() { return std::atan(T(1)) * T(4); }

template<typename T>
sf::Vector2<T> ceil(const sf::Vector2<T>& inp);
template<typename T>
sf::Vector2<T> floor(const sf::Vector2<T>& inp);

template<typename T>
T dot(const sf::Vector2<T>& first, const sf::Vector2<T>& second);

template<typename T>
T lengthSquared(const sf::Vector2<T>& inp);
template<typename T>
T length(const sf::Vector2<T>& inp);
template<typename T>
T distance(const sf::Vector2<T>& inp1, const sf::Vector2<T>& inp2);

template<typename T>
sf::Vector2<T> constrain(const sf::Rect<T>& rect, const sf::Vector2<T>& point);

}

#include "Math.inl"
