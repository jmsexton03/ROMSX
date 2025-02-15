
 .. role:: cpp(code)
    :language: c++

 .. _MeshRefinement:

Mesh Refinement
===============

REMORA allows both static and dynamic mesh refinement, as well as the choice of one-way or two-way coupling.

Note that any tagged region will be covered by one or more boxes.  The user may
specify the refinement criteria and/or region to be covered, but not the decomposition of the region into
individual grids.

See the `Gridding`_ section of the AMReX documentation for details of how individual grids are created.

.. _`Gridding`: https://amrex-codes.github.io/amrex/docs_html/ManagingGridHierarchy_Chapter.html

Static Mesh Refinement
----------------------

For static refinement, we control the placement of grids by specifying
the low and high extents (in physical space) of each box in the lateral
directions.   REMORA enforces that all refinement spans the entire vertical direction.

The following example demonstrates how to tag regions for static refinement.
In this first example, all cells in the region ((.15,.25,prob_lo_z)(.35,.45,prob_hi_z))
and in the region ((.65,.75,prob_lo_z)(.85,.95,prob_hi_z)) are tagged for
one level of refinement, where prob_lo_z and prob_hi_z are the vertical extents of the domain:

::

          amr.max_level = 1
          amr.ref_ratio = 2

          remora.refinement_indicators = box1 box2

          remora.box1.in_box_lo = .15 .25
          remora.box1.in_box_hi = .35 .45

          remora.box2.in_box_lo = .65 .75
          remora.box2.in_box_hi = .85 .95

In the example below, we refine the region ((.15,.25,prob_lo_z)(.35,.45,prob_hi_z))
by two levels of factor 3 refinement. In this case, the refined region at level 1 will
be sufficient to enclose the refined region at level 2.

::

          amr.max_level = 2
          amr.ref_ratio = 3 3

          remora.refinement_indicators = box1

          remora.box1.in_box_lo = .15 .25
          remora.box1.in_box_hi = .35 .45

And in this final example, the region ((.15,.25,prob_lo_z)(.35,.45,prob_hi_z))
will be refined by two levels of factor 3, but the larger region, ((.05,.05,prob_lo_z)(.75,.75,prob_hi_z))
will be refined by a single factor 3 refinement.

::

          amr.max_level = 2
          amr.ref_ratio = 3 3

          remora.refinement_indicators = box1 box2

          remora.box1.in_box_lo = .15 .25
          remora.box1.in_box_hi = .35 .45

          remora.box2.in_box_lo = .05 .05
          remora.box2.in_box_hi = .75 .75
          remora.box2.max_level = 1


Dynamic Mesh Refinement
-----------------------

Dynamically created tagging functions are based on runtime data specified in the inputs file.
These dynamically generated functions test on either state variables or derived variables
defined in REMORA_derive.cpp and included in the derive_lst in Setup.cpp.

Available tests include

-  “greater\_than”: :math:`field >= threshold`

-  “less\_than”: :math:`field <= threshold`

-  “adjacent\_difference\_greater”: :math:`max( | \text{difference between any nearest-neighbor cell} | ) >= threshold`

This example adds three user-named criteria –
hi\_rho: cells with density greater than 1 on level 0, and greater than 2 on level 1 and higher;
lo\_theta: cells with theta less than 300 that are inside the region ((.25,.25,.25)(.75,.75,.75));
and adv_diff: cells having a difference in the scalar of 0.01 or more from that of any immediate neighbor.
The first will trigger up to AMR level 3, the second only to level 1, and the third to level 2.
The third will be active only when the problem time is between 0.001 and 0.002 seconds.

Note that density and rhoadv_0 are the names of state variables, whereas theta is the name of a derived variable,
computed by dividing the variable named rhotheta by the variable named density.

::

          remora.refinement_indicators = hi_rho lo_theta advdiff

          remora.hi_rho.max_level = 3
          remora.hi_rho.value_greater = 1. 2.
          remora.hi_rho.field_name = density

          remora.lo_theta.max_level = 1
          remora.lo_theta.value_less = 300
          remora.lo_theta.field_name = rhotheta
          remora.lo_theta.in_box_lo = .25 .25 .25
          remora.lo_theta.in_box_hi = .75 .75 .75

          remora.advdiff.max_level = 2
          remora.advdiff.adjacent_difference_greater = 0.01
          remora.advdiff.field_name = rhoadv_0
          remora.advdiff.start_time = 0.001
          remora.advdiff.end_time = 0.002

Coupling Types
--------------

REMORA supports one-way and two-way coupling between levels; this is a run-time input

::

      remora.coupling_type = "OneWay" or "TwoWay"

By one-way coupling, we mean that between each pair of refinement levels,
the coarse level communicates data to the fine level to serve as boundary conditions
for the time advance of the fine solution. For cell-centered quantities,
and face-baced normal momenta on the coarse-fine interface, the coarse data is conservatively
interpolated to the fine level.

The interpolated data is utilized to specify ghost cell data (outside of the valid fine region).

By two-way coupling, we mean that in additional to interpolating data from the coarser level
to supply boundary conditions for the fine regions,
the fine level also communicates data back to the coarse level in two ways:

- The fine cell-centered data are conservatively averaged onto the coarse mesh covered by fine mesh.

- The fine momenta are conservatively averaged onto the coarse faces covered by fine mesh.

- A "reflux" operation is performed for all cell-centered data; this updates values on the coarser level outside of regions covered by the finer level.

Advected quantities which are advanced in conservation form will lose conservation with one-way coupling.
Two-way coupling ensures conservation of the advective contribution to all scalar updates but
does not account for loss of conservation due to diffusive or source terms.
