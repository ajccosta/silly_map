//Auxiliary class where < operator only cares about element at position Pos
template<size_t Pos, class... Ts>
struct tuple_x : public std::tuple<Ts...> { //Tuple where find only cares about first element
  std::tuple<Ts...> ts;
  tuple_x(Ts... ts) : ts(ts...) {}
  bool operator<(const tuple_x<Pos, Ts...> &tx2) const {
      return std::get<Pos>(ts) < std::get<Pos>(tx2.ts);
  }
  std::tuple<Ts...> get_tuple() { return ts; }
};

//Returns element at index n in set
template<typename T>
T at_index(std::set<T> s, size_t n) {
  typename std::set<T>::iterator it = s.begin();
  std::advance(it, n);
  return *it;
}

//Returns index of type T in ...Types
template <typename T, typename U=void, typename... Types>
constexpr size_t index_of_type() {
    return std::is_same<T, U>::value ? 0 : 1 + index_of_type<T, Types...>();
}

//Weird map where template is <key_index, types...>
// the key type will be the key at types...[key_index]
template<typename Key, class... Ts>
struct silly_map { //Tuple where find only cares about first element
  static constexpr size_t Pos = index_of_type<Key, Ts...>();
  std::set<tuple_x<Pos, Ts...>> tuple_xs;
  silly_map() {}
  void insert(Ts... ts) {
    tuple_xs.insert(tuple_x<Pos, Ts...>(ts...));
  }
  
  //Get tuple at index
  std::optional<std::tuple<Ts...>> find(const Key k) const {
      size_t i = 0;
      while(i < tuple_xs.size()) {
        std::tuple<Ts...> tup = at_index<tuple_x<Pos, Ts...>>(tuple_xs, i).get_tuple();
        Key e = std::get<Pos>(tup);
        if(reinterpret_cast<bool>(e == k)) {
          return tup;
        }
        i++;
      }
      return std::nullopt;
  }
};
