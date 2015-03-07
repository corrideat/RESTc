PROJECT(TEST)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

ADD_EXECUTABLE(RESTc_cURLRequestor RESTc_cURLRequestor.c)
TARGET_LINK_LIBRARIES(RESTc_cURLRequestor RESTc cmocka curl jansson)
ADD_TEST(cURLRequestor RESTc_cURLRequestor)

ADD_EXECUTABLE(RESTc_HashTable RESTc_HashTable.c)
TARGET_LINK_LIBRARIES(RESTc_HashTable RESTc cmocka curl jansson)
ADD_TEST(HashTable RESTc_HashTable)

ADD_EXECUTABLE(RESTc_JSONProcessor RESTc_JSONProcessor.c)
TARGET_LINK_LIBRARIES(RESTc_JSONProcessor RESTc cmocka curl jansson)
ADD_TEST(JSONProcessor RESTc_JSONProcessor)

ADD_EXECUTABLE(RESTc_String RESTc_String.c)
TARGET_LINK_LIBRARIES(RESTc_String RESTc cmocka curl jansson)
ADD_TEST(String RESTc_String)

ADD_EXECUTABLE(RESTc_StringConst RESTc_StringConst.c)
TARGET_LINK_LIBRARIES(RESTc_StringConst RESTc cmocka curl jansson)
ADD_TEST(StringConst RESTc_StringConst)

ADD_EXECUTABLE(RESTc_StringBucket RESTc_StringBucket.c)
TARGET_LINK_LIBRARIES(RESTc_StringBucket RESTc cmocka curl jansson)
ADD_TEST(StringBucket RESTc_StringBucket)
