/***************************************************************************
 *   Copyright (C) 2007 by                                                 *
 *                 Frank Gsellmann, Tobias Jaehnel, Carsten Kolassa        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TEST_BACKINGTREEMANAGER_H
#define TEST_BACKINGTREEMANAGER_H

#include <cppunit/extensions/HelperMacros.h>

/**
	@author Frank Gsellmann <frank.gsellmann@gmx.de>
*/
class Test_BackingtreeManager : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Test_BackingtreeManager);
        CPPUNIT_TEST(TestInstance);
        CPPUNIT_TEST(TestRegisterAndRemove);
    CPPUNIT_TEST_SUITE_END();

private:
    void TestInstance();
    void TestRegisterAndRemove();
};

#endif	// !TEST_BACKINGTREEMANAGER_H