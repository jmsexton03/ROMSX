---
title: 'REMORA: Regional Modeling of Oceans Refined Adaptively (built on AMReX)'

tags:
  - C++
  - ocean modeling
  - mesoscale

authors:
  - name: Hannah Klion
    orcid: 0000-0003-2095-4293
    corresponding: true
    affiliation: 1
  - name: Jean Sexton
    orcid: 0000-0003-2551-1678
    corresponding: false
    affiliation: 1
  - name: Ann Almgren
    orcid: 0000-0003-2103-312X
    corresponding: false
    affiliation: 1
  - name: Robert Hetland
    orcid: 0000-0001-9531-2119
    corresponding: false
    affiliation: 2
  - name: Kyle Hinson
    orcid: 0000-0002-2737-2379
    corresponding: false
    affiliation: 2
  - name: Iulian Grindeanu
    orcid: 0000-0002-0264-8236
    corresponding: false
    affiliation: 3
  - name: Vijay Mahadevan
    orcid: 0000-0002-3337-2607
    corresponding: false
    affiliation: 3

affiliations:
 - name: Lawrence Berkeley National Laboratory
   index: 1
 - name: Pacific Northwest Energy Laboratory
   index: 2
 - name: Argonne National Laboratory
   index: 3

date: March 2024

bibliography: paper.bib
---

# Summary

The REMORA code is a new model that simulates the mesoscale and microscale
dynamics of the ocean using the latest high-performance computing architectures.
It employs hierarchical parallelism using an MPI+X model, where X may be OpenMP on
multicore CPU-only systems, or CUDA, HIP, or SYCL on GPU-accelerated systems.
It is able to be built and run in both single and double precision.
REMORA is built on AMReX [@AMReX:JOSS; @AMReX:IJHPCA],
a block-structured adaptive mesh refinement (AMR) software framework that
provides the underlying performance-portable software infrastructure for block-structured mesh operations.
REMORA, like ROMS, is a regional model, meaning that it simulates the ocean dynamics on
a less-than-global scale, and as such requires boundary conditions derived from data
or from larger-scale models.
The REMORA development is funded by the US Department of Energy's Office of Science
through the Science Discovery through Advanced Computing (SciDAC) partnership program.

# REMORA Features

### Evolution Equations

REMORA solves the ...
and incorporates temperature, salinity, and an arbitrary scalar which can be advected and diffused.

### Turbulence/Mixing Schemes

### Time and Space Discretization and Terrain

The time discretization in REMORA is the ... model as described on ROMS web page.
In each time step, the depth-averaged equations are first advanced to determine mean quanitities
and ocean height, then the full three-dimensional equations are evolved for velocity and scalars.

The spatial discretization in REMORA uses the classic Arakawa C-grid with
scalar quantities at cell centers and normal velocities at cell faces.
Bathymetry is included in the discretizations as described here.

For simulations over complex bathymetry ...
The model includes capability for application
of some common map projections (e.g., Lambert Conformal, Mercator).
The advection terms may be calculated using second- through sixth-order accurate
spatial discretizations, including both centered difference and upwind
schemes.

### Dynamic and Static Mesh Refinement

REMORA supports both static and dynamic (adaptive) mesh refinement,
with subcycling in time at finer levels of refinement.

### Physical Forcings and Boundary Conditions

Physical forcings include Coriolis and wind stress forcing.
Lateral boundary conditions can be specified as periodic, inflow/outflow,
or time-varying values read in from external files in netcdf format.
The initial data can be specified by the user or read in from netcdf files.

# Statement of need

Most widely used ocean modeling codes today do not have the
ability to use GPU acceleration, which limits their ability to
efficiently utilize current and next-generation high performance computing
architectures.  REMORA provides an ocean modeling capability that runs on the latest high-performance
computing architectures, from laptops to supercomputers,
whether CPU-only or GPU-accelerated.  In addition, REMORA is based on AMReX,
a modern, well-supported adaptive mesh refinement (AMR) library,
which provides a performance portable interface that shields REMORA
from most of the detailed changes needed to adapt to new systems.
The active and large developer community contributing to AMReX helps ensure
that REMORA will continue to run efficiently as architectures and operating systems
evolve.

# Acknowledgements

Funding for this work was provided by the U.S. Department of Energy
Office of Energy Efficiency and Renewable Energy Wind Energy Technologies Office.
We acknowledge the help of the AMReX team
in developing and supporting new AMReX features needed by ERF.
The work at LBNL was supported by the U.S. Department of Energy
under contract No. DE-AC02-05CH11231.
The work at PNNL was supported by the U.S. Department of Energy
under contract No.

# References
