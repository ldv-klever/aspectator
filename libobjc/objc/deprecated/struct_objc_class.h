/* This structure used to be public, but is now private to the runtime.  */
#define __objc_STRUCT_OBJC_CLASS_defined

/*
** The compiler generates one of these structures for each class.  
** 
** This structure is the definition for classes. 
** 
** This structure is generated by the compiler in the executable and used by
** the run-time during normal messaging operations.  Therefore some members
** change type. The compiler generates "char* const" and places a string in
** the following member variables:  super_class. 
*/
struct objc_class {     
  MetaClass           class_pointer;          /* Pointer to the class's
                                                meta class. */
  struct objc_class*  super_class;            /* Pointer to the super 
                                                class. NULL for class 
                                                Object. */
  const char*         name;                   /* Name of the class. */
  long                version;                /* Unknown. */
  unsigned long       info;                   /* Bit mask.  See class masks 
                                                defined above. */
  long                instance_size;          /* Size in bytes of the class.  
                                                The sum of the class 
						definition and all super 
						class definitions. */
#ifdef _WIN64
  /* We pad the structure manually to prevent warning when -Wpadded
     is used.  The compiler automatically pads the structures that it
     generates, so this manually padded structure still matches the one
     generated by the compiler, but if we don't pad manually, -Wpadded
     detects that padding is being added and generates annoying warnings.
     This hack is necessary as on LLP64 targets sizeof (long) isn't equal
     to sizeof (void *).  */
  long pad;
#endif
  struct objc_ivar_list* ivars;               /* Pointer to a structure that
                                                describes the instance 
                                                variables in the class
                                                definition.  NULL indicates
                                                no instance variables.  Does
                                                not include super class
                                                variables. */
  struct objc_method_list*  methods;          /* Linked list of instance
                                                methods defined for the 
                                                class. */
  struct sarray *    dtable;                  /* Pointer to instance 
					         method dispatch table. */  
  struct objc_class* subclass_list;           /* Subclasses */
  struct objc_class* sibling_class;

  struct objc_protocol_list *protocols;	      /* Protocols conformed to */
  void* gc_object_type;
};

