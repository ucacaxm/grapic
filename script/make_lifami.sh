#!/bin/bash
pause()
{
#	read -p "Press [Enter] to continue..."
	printf "Continue ...\n"
}
pwd
mkdir -p apps/LIFAMI
cp  apps/start/main_start.cpp apps/LIFAMI/TPComplex.cpp
cp  apps/start/main_start.cpp apps/LIFAMI/TPInterpolation.cpp
cp  apps/start/main_start.cpp apps/LIFAMI/TPParticle.cpp
cp  apps/start/main_start.cpp apps/LIFAMI/TPParticleSpring.cpp
cp  apps/start/main_start.cpp apps/LIFAMI/TPGameOfLife.cpp
cp  apps/start/main_start.cpp apps/LIFAMI/TPWolfRabbit.cpp
cp  apps/start/main_start.cpp apps/LIFAMI/TPColorInsects.cpp
cp  apps/start/main_start.cpp apps/LIFAMI/TPIceScream.cpp
echo "ls apps/LIFAMI"
ls apps/LIFAMI