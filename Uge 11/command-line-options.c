$ ./fauxgrep assert A4/ref
A4/ref/test_error2.c:5: #include <assert.h>

A4/ref/test_error2.c:28:
assert(transducers_link_source(&s[0], dummy_source, NULL) == 0);

A4/ref/test_error2.c:29:
assert(transducers_dup(&s[1], &s[2], s[0]) == 0);

A4/ref/test_error2.c:30:
assert(transducers_link_2(&s[3], dummy_transducer_2, NULL, s[1], s[2]) == 0);

A4/ref/test_error2.c:31:
assert(transducers_link_2(&s[4], dummy_transducer_2, NULL, s[2], s[3]) != 0);

A4/ref/test_error1.c:5: #include <assert.h>
A4/ref/test_error1.c:28:
assert(transducers_link_source(&s[0], dummy_source, NULL) == 0);

A4/ref/test_error1.c:29:
assert(transducers_link_1(&s[1], dummy_transducer_1, NULL, s[0]) == 0);

A4/ref/test_error1.c:30:
assert(transducers_link_1(&s[2], dummy_transducer_1, NULL, s[0]) != 0);

A4/ref/transducers.c:4: #include <assert.h>