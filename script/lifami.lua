
-- Do not remove this line!
dofile "../grapic.lua"
dofile "../premake4.lua"


make_project( "LIFAMI_1_Complex", "apps/LIFAMI/TPComplex.cpp" )
make_project( "LIFAMI_2_Interpolation", "apps/LIFAMI/TPInterpolation.cpp" )
make_project( "LIFAMI_3_Particle", "apps/LIFAMI/TPParticle.cpp" )
make_project( "LIFAMI_4_ParticleSpring", "apps/LIFAMI/TPParticleSpring.cpp" )
make_project( "LIFAMI_5_GameOfLife", "apps/LIFAMI/TPGameOfLife.cpp" )
make_project( "LIFAMI_6_WolfRabbit", "apps/LIFAMI/TPWolfRabbit.cpp" )
make_project( "LIFAMI_7_ColorInsects", "apps/LIFAMI/TPColorInsects.cpp" )
make_project( "LIFAMI_8_IceScream", "apps/LIFAMI/TPIceScream.cpp" )
