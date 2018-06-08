

##boost:checked_delete

下面是boost::checked_delete的实现，在Boost Utility库中，如果T只声明了而未定义，sizeof(T)将返回0，此时checked_delete将因为声明-1个成员的数组而引发错误。


```cpp
template<class T> inline void checked_delete(T * x)
{
  typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
  (void) sizeof(type_must_be_complete);
  delete x;
}

template<class T> inline void checked_array_delete(T * x)
{
  typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
  (void) sizeof(type_must_be_complete);
  delete [] x;
}
```

[https://blog.csdn.net/hzyong_c/article/details/7777344](https://blog.csdn.net/hzyong_c/article/details/7777344)