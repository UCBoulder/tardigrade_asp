.. _changelog:


#########
Changelog
#########

******************
0.1.0 (unreleased)
******************

Breaking changes
================
- Remove linear elasticity, which now lives in stress tools (:issue:`1`, :merge:`9`). By `Kyle Brindley`_.
- Changed the getter functions to use constant pointers rather than copied values (:merge:`22`). By `Nathan Miller`_.
- Re-named surface energy density functions to surface adhesion energy density (:merge:`23`). By `Nathan Miller`_.

Internal Changes
================
- Change the build system to more closely mimic the current state of cpp_stub (:merge:`15`). By `Nathan Miller`_.
- Change the project to use boost 1.65 (:merge:`17`). By `Nathan Miller`_.
- Change the project to use error_tools > 0.5.1 (:merge:`19`). By `Nathan Miller`_.
- Begin incorporation of new error_tools stack trace capability (:merge:`20`). By `Nathan Miller`_.
- Added getter and setter functions for the private variables (:merge:`21`). By `Nathan Miller`_.
- Added a function for the particle overlap calculation which uses a non-local micro-deformation
  rather than the gradient of the micro deformation (:merge:`24`). By `Nathan Miller`_.
- Added a general bounding box function to reduce duplicated code (:merge:`26`). By `Nathan Miller`_.
- Added a function for the particle overlap calculation which uses an arbitrary, "basis," i.e.,
  the reference point for the Taylor series computation of the non-local micro-deformation which will
  allow for random neighboring particles (:merge:`27`). By `Nathan Miller`_.
- Changed the overlap surface energy to be a surface density rather than a volumetric density (:merge:`31`). By `Nathan Miller`_.
- Changed the setter functions to use a parameter-less setter for user defined functions, and a value taking setter that can be called by developers. (:merge:`36`). By `Nathan Miller`_.
- Added getter functions for the gradients of the current distance vector. (:merge:`36`). By `Nathan Miller`_.
- Added the gradients of the non-local deformation w.r.t. the incoming measures. (:merge:`36`). By `Nathan Miller`_.
- Added the gradients of the current distance vector w.r.t. the incoming measures. (:merge:`36`). By `Nathan Miller`_.
- Added the gradients of the current normal vector w.r.t. the incoming measures. (:merge:`36`). By `Nathan Miller`_.
- Added the gradients of the surface adhesion energy density w.r.t. the incoming measures. (:merge:`36`). By `Nathan Miller`_.
- Added the non-zero hessians of the non-local micro-deformation w.r.t. the incoming measures. (:merge:`36`). By `Nathan Miller`_.
- Allow for the version to be specified when doing FetchContent builds (:pull:`3`). By `Nathan Miller`_.

New Features
============
- Added linear elasticity for the internal particle (:merge:`3`). By `Nathan Miller`_.
- Added the calculation of the current distance between two points in the particles (:merge:`4`). By `Nathan Miller`_.
- Added the calculation of the decomposition of a vector into normal and tangential components w.r.t. another vector (:merge:`4`). By `Nathan Miller`_.
- Added the calculation of the linear traction separation energy (:merge:`4`). By `Nathan Miller`_.
- Added the calculation of the gradients of the traction separation energy w.r.t. the parameters (:merge:`5`). By `Nathan Miller`_.
- Added the calculation of the traction separation traction - Cauchy stress traction constraint equation (:merge:`6`). By `Nathan Miller`_.
- Added the calculation of Nanson's relation for mapping reference areas to the current configuration (:merge:`7`). By `Nathan Miller`_.
- Added the calculation of the Lagrangian for the overlap particle (:merge:`8`). By `Nathan Miller`_.
- Extended the Lagrangian for the overlap particle to have a radius other than 1 (:merge:`8`). By `Nathan Miller`_.
- Extended the number of gradients computed in the Lagrangian for the overlap particle (:merge:`8`). By `Nathan Miller`_.
- Added the calculation of the amount of overlap of a non-local and local particles (:merge:`8`). By `Nathan Miller`_.
- Added the calculation of the gradients for the local and non-local particles (:merge:`8`). By `Nathan Miller`_.
- Added the calculation of the fourth-order gradients for the Lagrangian for the overlap particle. This is required for the constraint equation. (:merge:`8`). By `Nathan Miller`_.
- Added the calculation of the third-order gradients for the solution of the overlap distance. This is required for the constraint equation. (:merge:`8`). By `Nathan Miller`_.
- Added the decomposition of a sphere for the purposes of integration and contact detection. (:merge:`10`). By `Nathan Miller`_.
- Added the capability to integrate 2D quadratic elements and surface meshes composed of quadratic elements. (:merge:`11`). By `Nathan Miller`_.
- Added a more general form of the distance calculation to support random deformations. (:merge:`12`). By `Nathan Miller`_.
- Initial commit of the asp base class with a linear elastic local particle energy definition. (:merge:`14`). By `Nathan Miller`_.
- Added the computation of the traction separation energy at a single surface point. (:merge:`15`). By `Nathan Miller`_.
- Changed the interface to the surface energy such that it sets an internal variable rather than returning a value (:merge:`16`). By `Nathan Miller`_.
- Added the computation of the points on the surface of the local and non-local particles in the reference and current configurations (:merge:`25`). By `Nathan Miller`_.
- Added the ability to detect if points are in a bounding box (:merge:`27`). By `Nathan Miller`_.
- Added the capability to compute the overlap energy (:merge:`28`). By `Nathan Miller`_.
- Added the computation of the traction for a linear traction (:merge:`29`). By `Nathan Miller`_.
- Added the computation of the surface adhesion traction (:merge:`29`). By `Nathan Miller`_.
- Added the computation of the surface overlap traction (:merge:`30`). By `Nathan Miller`_.
- Added the functions to reset internally computed values at the correct times (:merge:`31`). By `Nathan Miller`_.
- Added ability to compute the normals (reference and current) of a local particle at arbitrary surface points rather than just the current points (:merge:`31`). By `Nathan Miller`_.
- Added all required functions for the computation of the local particle's energy (:merge:`32`). By `Nathan Miller`_.
- Added the computation of the local particle's energy (:merge:`32`). By `Nathan Miller`_.
- Added the assembly of the surface adhesion quantities (:merge:`33`). By `Nathan Miller`_.
- Added the computation of the surface overlap thickness (:merge:`34`). By `Nathan Miller`_.
- Added the assembly of the overlap quantities (:merge:`35`). By `Nathan Miller`_.

Bug Fixes
=========
- Removed whitespace trailing after add_library (:pull:`1`). By `Nathan Miller`_.
