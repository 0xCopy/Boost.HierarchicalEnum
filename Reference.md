Definition of a HierarchicalEnum
================================

Simple case
-----------

    BOOST_HIERARCHICAL_ENUM( EnumName, Enumerators )

where:

- EnumName is the name of the enum t be declared
- Enumerators is a Boost.Preprocessor Sequence (Enumerator1)(Value2)...(ValueN)
- ValueI is a Boost.Preprocessor Tuple ( ValueName, 

Namespaces
----------

Unfortunately, the current implementation of those macros define some template specializations that must live in the ::boost namespace.
This doesn't allow you to define a HierarchicalEnum inside your own namespace.
To define a HierarchicalEnum inside a namespace, you have to use:

    BOOST_HIERARCHICAL_ENUM_NS( Namespaces, EnumName, Values )
    
where:

- Namespaces is the Boost.Preprocessor Sequence of the namespaces where the HierarchicalEnum must be declared, from the outer to the inner one.
- EnumName is as for BOOST_HIERARCHICAL_ENUM
- Values is as for BOOST_HIERARCHICAL_ENUM

Queries about a HierarchicalEnum
================================

Run-time, compile-time and mixed-time
-------------------------------------

Each function comes in two versions: the classic run-time version, whose arguments are instances of the enum, and the template compile-time version, which is in fact a metafunction.
Additionally, functions taking two or more arguments have mixed-time versions, which take some of their arguments as run-time arguments, and the other arguments as template arguments.

Predicates, single elements and collections
-------------------------------------------

There are three return types for the functions of the library: boolean, (optional) single element, and collection.

The run-time and mixed-time versions of functions returning a boolean return a bool.
The compile-time version returns boost::mpl::true_ or boost::mpl::false_.

Functions definitions
---------------------

### is_child_of
A is_child_of B iif A is listed in B's children list

### children_of
children_of A is the set os Bs such that B is_child_of A

### is_strict_descendant_of
A is_strict_descendant_of B iif A is_child_of B or if there is a C such that A is_child_of C and C is_strict_descendant_of B

### is_descendant_of
A is_descendant_of B iif A == B or A is_strict_descendant_of B

### is_father_of
A is_father_of B iif B is_child_of A

### has_father
A has_father iif A is in a children list

### father_of
Only valid if A has_father. Overwise, the run-time version will throw, and the compile-time version will not compile.
father_of A is the B such that B is_father_of A

### is_ascendant_of
A is_ascendant_of B iif B is_descendant_of A

### is_strict_ascendant_of
A is_strict_ascendant_of B iif B is_strict_descendant_of A

### common_ascendants_of
common_ascendants_of A and B is the set of Cs such that C is_ascendant_of A and C is_ascendant_of B

### strict_common_ascendants_of
strict_common_ascendants_of A and B is the set of Cs such that C is_strict_ascendant_of A and C is_strict_ascendant_of B

### have_common_ascendant
A and B have_common_ascendant iif common_ascendants_of A and B is not empty

### have_strict_common_ascendant
A and B have_strict_common_ascendant iif strict_common_ascendants_of A and B is not empty

### first_common_ascendant_of
first_common_ascendant_of A and B is the C in common_ascendants_of A and B such that for any D in common_ascendants_of A and B, D is_ascendant_of C

### first_strict_common_ascendant_of
first_strict_common_ascendant_of A and B is the C in strict_common_ascendants_of A and B such that for any D in strict_common_ascendants_of A and B, D is_ascendant_of C
