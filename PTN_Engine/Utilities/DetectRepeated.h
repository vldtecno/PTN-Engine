/*
 * This file is part of PTN Engine
 *
 * Copyright (c) 2018-2019 Eduardo Valgôde
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <memory>
#include <vector>

namespace ptne::utility
{

template <typename T, class E>
void detectRepeated(std::vector<std::weak_ptr<T>> items)
{
	auto equals = [](const std::weak_ptr<T> &t, const std::weak_ptr<T> &u)
	{ return !t.owner_before(u) && !u.owner_before(t); };

	std::ranges::sort(items, std::owner_less<std::weak_ptr<T>>());

	if (items.end() - std::unique(items.begin(), items.end(), equals) > 0)
	{
		throw E();
	}
}

template <class T, class E>
void detectRepeatedNames(std::vector<T> items)
{
	std::ranges::sort(items);
	if (items.end() - std::unique(items.begin(), items.end()) > 0)
	{
		throw E();
	}
}

} // namespace ptne::utility
