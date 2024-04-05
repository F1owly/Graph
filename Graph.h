#pragma once
#include <unordered_map>

namespace graph {
	template<typename Key, typename Value, typename Weight>
	class Graph {
	private:
		using key_type = Key;
		using value_type = Value;
		using weight_type = Weight;

		class Node {
		private:
			value_type value; //значение, хранимое в вершине
			unordered_map<key_type, weight_type>edges; //вершины, в которые можно попасть из этой вершины и вес соответствующего ребра

		public:
			Node() = default;

			Node(value_type value_) : value(value_)  {}

			Node(const Node& node) {
				value = node.value;
				edges = node.edges;
			}

			void clear() {
				value = 0;
			}

			Node(const Node&& node) {
				value = node.value;
				edges = node.edges;
				node.clear();
			}
			
			Node& operator=(const Node& node) {
				value = node.value;
				edges = node.edges;
				return *this;
			}

			Node& operator=(const Node&& node) {
				if (moved != *this) {
					value = node.value;
					edges = node.edges;
					moved.clear();
				}
				return *this;
			}

			bool empty() const{
				return edges.empty();
			}

			size_t size() const{
				return edges.size();
			}

			value_type& value() const{
				return value;
			}

			const value_type& value() const {
				return value;
			}

			using unordered_map<key_type, weight_type>::iterator = iterator;
			using unordered_map<key_type, weight_type>::const_iterator = const_iterator;

			iterator begin() {
				return edges.begin();
			}

			const_iterator cbegin() {
				return edges.cbegin();
			}

			iterator end() {
				return edges.end();
			}

			const_iterator cend() {
				return edges.cend();
			}

		};

		unordered_mup<key_type, Node>nodes; //вершины, принадлежащие этому графу и их псевдонимы


	};


}