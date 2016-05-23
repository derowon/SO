#ifndef _SHAREDFUNCTIONS_H
#define _SHAREDFUNCTIONS_H

#include "packages.h"

void answer(Package * data);
Package * materias_db(Package * data);
Package * misMaterias_db(Package * data);
Package * subscribeSubject_db(Package * data);
Package * cancelSubscription_db(Package * data);
Package * correlatives_db(Package * data);
Package * checkUser_db(Package * data);
Package * cupo_db(Package * data);
#endif