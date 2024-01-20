# [vec.h](src/collections/vec.h)
## Usage:
Create a new Vec with:
```c
Arena arena = {0};
VEC(int) vec = {0};
vec_init(&vec, &arena);
```

Then you can push elements to the vector.
```c
vec_push(&vec, 69);
vec_push(&vec, 420);
```
*/
# [hm.h](src/collections/hm.h)
## Usage
Create a new HashMap with:
```c
Arena arena = {0};
HashMap hm = hm_create(&arena);
hm_reserve(&hm, 10); // optional
```

Then you can add elements by hash to the HashMap.
```c
hm_insert(&set, str_hash(STR("Hello")), HashValue(i64, 69));
hm_insert(&set, str_hash(STR("World")), HashValue(i64, 420));
```

Now you can get the values by passing in the hash of the element.
```c
hm_get(&set, str_hash(STR("Hello")))->as.i64;
hm_get(&set, str_hash(STR("World")))->as.i64;
```
*/
# [set.h](src/collections/set.h)
## Usage
Create a new Set with:
```c
Arena arena = {0};
Set set = set_create(&arena);
```

Then you can add elements by hash to the Set.
```c
set_add(&set, str_hash(STR("Hello")));
set_add(&set, str_hash(STR("World")));
```

Then you can test if an element is in the Set.
```c
set_contains(&set, str_hash(STR("Hello"))) == true;
set_contains(&set, str_hash(STR("World"))) == true;
```
*/
