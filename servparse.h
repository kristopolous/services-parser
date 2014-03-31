/*
 * BSD license by inclusion of servparse.cpp from NetBSD,
 * see the file servparse.cpp for details.  This file
 * assumes the most permissive available and applicable 
 * license given the restrictions therein.
 */

#include <string>
using namespace std;

string findByPort(int port, string type/* = "tcp"*/);
int findByName(string name);
