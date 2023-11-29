#include <ROMSX.H>

using namespace amrex;

void
ROMSX::t3dmix  (const Box& bx,
                Array4<Real> t,
                Array4<Real> diff2, Array4<Real> Hz,
                Array4<Real> pm, Array4<Real> pn,
                Array4<Real> pmon_u, Array4<Real> pnom_v,
                int nrhs, int nnew,
                const amrex::Real dt_lev)
{
    //-----------------------------------------------------------------------
    //  Add in harmonic diffusivity s terms.
    //-----------------------------------------------------------------------

    Box xbx(bx); xbx.surroundingNodes(0);
    Box ybx(bx); ybx.surroundingNodes(1);

    FArrayBox fab_FX(xbx,1,amrex::The_Async_Arena());
    FArrayBox fab_FE(ybx,1,amrex::The_Async_Arena());

    fab_FX.template setVal<RunOn::Device>(0.);
    fab_FE.template setVal<RunOn::Device>(0.);

    auto FX=fab_FX.array();
    auto FE=fab_FE.array();

    ParallelFor(xbx,
        [=] AMREX_GPU_DEVICE (int i, int j, int k)
        {
            const amrex::Real cff = 0.25 * (diff2(i,j,0) + diff2(i-1,j,0)) * pmon_u(i,j,0);
            FX(i,j,k) = cff * (Hz(i,j,k)+Hz(i+1,j,k))*(t(i,j,k,nrhs)-t(i-1,j,k,nrhs));
        });

    ParallelFor(ybx,
        [=] AMREX_GPU_DEVICE (int i, int j, int k)
        {
            const amrex::Real cff=0.25*(diff2(i,j,0)+diff2(i,j-1,0)) * pnom_v(i,j,0);
            FE(i,j,k) = cff * (Hz(i,j,k) + Hz(i,j-1,k)) * (t(i,j,k,nrhs) - t(i,j-1,k,nrhs));
        });

    /*
     Time-step harmonic, S-surfaces diffusion term.
    */
    amrex::ParallelFor(bx,
        [=] AMREX_GPU_DEVICE (int i, int j, int k)
        {
            const amrex::Real cff=dt_lev*pm(i,j,0)*pn(i,j,0);
            const amrex::Real cff1=cff*(FX(i+1,j  ,k)-FX(i,j,k));
            const amrex::Real cff2=cff*(FE(i  ,j+1,k)-FE(i,j,k));
            const amrex::Real cff3=cff1+cff2;

            t(i,j,k,nnew) += cff3;
        });
}
