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
#ifndef TEST_BACKINGTREE_H
#define TEST_BACKINGTREE_H

#include <cppunit/extensions/HelperMacros.h>

/**
	@author Frank Gsellmann <frank.gsellmann@gmx.de>
*/
class Test_Backingtree : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(Test_Backingtree);
        CPPUNIT_TEST(TestConstructor);
        CPPUNIT_TEST(TestEqual);
        CPPUNIT_TEST(TestSearchPath);
        CPPUNIT_TEST(TestGetCachePath);
    CPPUNIT_TEST_SUITE_END();

private:
    void TestConstructor();
    void TestEqual();
    void TestSearchPath();
    void TestGetCachePath();
};

#endif	// !TEST_BACKINGTREE_H