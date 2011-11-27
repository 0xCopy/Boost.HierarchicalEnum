Functional improvements
=======================

- Use a DAG instead of a tree (will be more symetric with children)
    - Parents instead of father
    - Warning: detect cycles at compilation time: this must be a DAG
        - Check if BGL can help
- Check that there are no "son(s)" but only "chid(ren)"
        
Technical improvements
======================

- See BOOST_FUSION_ADAPT_STRUCT_FILLER_0 to remove the double parenthesis in BOOST_HIERARCHICAL_ENUM.
- See BOOST_FUSION_DEFINE_STRUCT for how to deal with namespaces.

How to become part of Boost?
============================

- Be accepted
    - Be reviewed
        - Generate enough interest
            - Write easy to read tutorial and introduction
            - Write a mail that makes people want to click on the link
    - Quality
        - Write good documentation
        - Boostify the code
            - All conventions
            - Correct usage of compile-time functions and types
    - Reactivity
        - Launch the process only when I have engouh free time in front of me
        - Be prepared to spend a few hours each dayt on the project
    - Honesty
        - Be clear about the "candidate" status
- Merge to Boost's svn

Add to the readme
=================

small, convenient, header-only, easy to use, easy to learn

should be used when the State pattern is overkill
