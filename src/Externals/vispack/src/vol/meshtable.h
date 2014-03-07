/*
// ******************************************************************
// VISPack. Copyright (c) 1994-2000 Ross Whitaker rtw@utk.edu       *
// For conditions of distribution and use, see the file LICENSE.txt *
// accompanying this distribution.                                  *
// ******************************************************************

// $Id: meshtable.h,v 1.1.1.1 2003/02/12 16:51:54 whitaker Exp $
*/



int num_tri[256] =
{0,1,1,2,1, 2,2,3,1,2,
 2,3,2,3,3, 2,1,2,2,3,
 2,3,3,4,2, 3,3,4,3,4,
 4,3,1,2,2, 3,2,3,3,4,
 2,3,3,4,3, 4,4,3,2,3,
 3,2,3,4,4, 3,3,4,4,3,
 4,3,3,2,1, 2,2,3,2,3,
 3,4,2,3,3, 4,3,4,4,3,
 2,3,3,4,3, 4,4,3,3,4,
 4,3,4,3,3, 2,2,3,3,4,
 3,4,2,3,3, 4,4,3,4,3,
 3,2,3,4,4, 3,4,3,3,2,
 4,3,3,2,3, 2,2,1,1,2,
 2,3,2,3,3, 4,2,3,3,4,
 3,4,4,3,2, 3,3,4,3,4,
 4,3,3,2,4, 3,4,3,3,2,
 2,3,3,4,3, 4,4,3,3,4,
 4,3,4,3,3, 2,3,4,4,3,
 4,3,3,2,4, 3,3,2,3,2,
 2,1,2,3,3, 4,3,4,4,3,
 3,4,4,3,2, 3,3,2,3,4,
 4,3,4,3,3, 2,4,3,3,2,
 3,2,2,1,3, 4,4,3,4,3,
 3,2,4,3,3, 2,3,2,2,1,
 2,3,3,2,3, 2,2,1,3,2,
 2,1,2,1,1, 0};

int meshdata[] =
{1,4,9,            
10,2,1,            
2,4,9,            2,9,10,            
12,3,2,            
1,4,9,            12,3,2,            
10,3,1,            3,10,12,            
9,10,12,            4,9,12,            3,4,12,            
11,4,3,            
11,9,1,            1,3,11,            
11,4,3,            10,2,1,            
9,10,11,            11,10,3,            2,3,10,            
12,4,2,            12,11,4,            
12,11,9,            12,9,2,            1,2,9,            
12,11,10,            1,10,11,            11,4,1,            
12,11,9,            9,10,12,            
9,8,5,            
1,4,5,            8,5,4,            
10,2,1,            9,8,5,            
2,4,8,            2,8,10,            10,8,5,            
12,3,2,            9,8,5,            
12,3,2,            1,4,5,            8,5,4,            
9,8,5,            10,3,1,            3,10,12,            
10,8,5,            3,8,10,            3,4,8,            3,10,12,            
11,4,3,            9,8,5,            
1,3,5,            8,5,3,            11,8,3,            
11,4,3,            10,2,1,            9,8,5,            
2,3,11,            11,10,2,            8,10,11,            10,8,5,            
9,8,5,            12,4,2,            12,11,4,            
11,8,5,            2,5,11,            12,11,2,            1,2,5,            
9,8,5,            12,11,10,            1,10,11,            11,4,1,            
12,11,10,            8,10,11,            10,8,5,            
5,6,10,            
1,4,9,            5,6,10,            
6,2,1,            1,5,6,            
2,4,6,            4,5,6,            9,5,4,            
5,6,10,            12,3,2,            
12,3,2,            1,4,9,            5,6,10,            
5,3,1,            3,5,12,            5,6,12,            
5,6,9,            9,6,4,            4,6,12,            3,4,12,            
5,6,10,            11,4,3,            
5,6,10,            11,9,1,            1,3,11,            
11,4,3,            6,2,1,            1,5,6,            
2,3,11,            11,5,2,            2,5,6,            11,9,5,            
5,6,10,            12,11,4,            12,4,2,            
5,6,10,            12,11,9,            12,9,2,            1,2,9,            
5,6,12,            4,5,12,            5,4,1,            12,11,4,            
12,11,9,            12,9,5,            5,6,12,            
10,9,8,            10,8,6,            
8,6,4,            1,4,6,            1,6,10,            
8,6,2,            9,8,2,            9,2,1,            
8,6,4,            2,4,6,            
12,3,2,            10,9,8,            10,8,6,            
12,3,2,            8,6,4,            1,4,6,            1,6,10,            
9,8,1,            1,8,12,            12,3,1,            12,8,6,            
8,6,4,            4,6,12,            3,4,12,            
11,4,3,            10,8,6,            10,9,8,            
1,6,10,            11,6,1,            11,8,6,            1,3,11,            
11,4,3,            8,6,2,            9,8,2,            9,2,1,            
8,6,2,            2,3,8,            11,8,3,            
10,9,6,            9,8,6,            12,11,4,            12,4,2,            
1,2,10,            11,8,6,            12,11,6,            
9,4,1,            11,8,6,            12,11,6,            
11,8,6,            12,11,6,            
6,7,12,            
6,7,12,            1,4,9,            
6,7,12,            10,2,1,            
6,7,12,            2,9,10,            2,4,9,            
7,3,2,            2,6,7,            
1,4,9,            7,3,2,            2,6,7,            
7,3,1,            1,6,7,            10,6,1,            
6,7,10,            10,7,4,            4,9,10,            3,4,7,            
6,7,12,            11,4,3,            
6,7,12,            1,3,11,            11,9,1,            
11,4,3,            6,7,12,            10,2,1,            
6,7,12,            9,10,11,            11,10,3,            2,3,10,            
6,4,2,            4,6,11,            6,7,11,            
1,2,9,            2,7,9,            11,9,7,            2,6,7,            
6,7,10,            10,7,1,            1,7,11,            11,4,1,            
9,10,11,            11,10,6,            6,7,11,            
9,8,5,            6,7,12,            
6,7,12,            1,4,5,            8,5,4,            
6,7,12,            10,2,1,            9,8,5,            
6,7,12,            2,4,8,            2,8,10,            10,8,5,            
9,8,5,            7,3,2,            2,6,7,            
6,3,2,            3,6,7,            1,4,5,            8,5,4,            
9,8,5,            7,3,1,            1,6,7,            10,6,1,            
10,6,5,            8,7,3,            3,4,8,            
11,4,3,            9,8,5,            6,7,12,            
6,7,12,            1,3,5,            3,5,8,            11,8,3,            
6,7,12,            9,8,5,            11,4,3,            10,2,1,            
2,3,12,            11,8,7,            10,6,5,            
9,8,5,            6,4,2,            4,6,11,            6,7,11,            
11,8,7,            6,5,1,            1,2,6,            
11,8,7,            10,6,5,            9,4,1,            
10,6,5,            11,8,7,            
5,7,12,            12,10,5,            
1,4,9,            5,7,12,            12,10,5,            
1,5,7,            7,2,1,            2,7,12,            
2,7,12,            9,7,2,            5,7,9,            2,4,9,            
3,5,7,            10,5,3,            10,3,2,            
1,4,9,            3,5,7,            10,5,3,            10,3,2,            
7,3,1,            1,5,7,            
3,5,7,            3,4,5,            9,5,4,            
11,4,3,            12,10,5,            5,7,12,            
11,9,3,            1,3,9,            5,7,12,            12,10,5,            
11,4,3,            1,5,7,            7,2,1,            2,7,12,            
2,3,12,            11,9,7,            5,7,9,            
4,7,11,            10,7,4,            5,7,10,            10,4,2,            
1,2,10,            11,9,7,            5,7,9,            
1,5,7,            1,7,11,            11,4,1,            
11,9,7,            5,7,9,            
12,10,9,            12,9,7,            9,8,7,            
1,4,10,            4,7,10,            12,10,7,            8,7,4,            
12,2,1,            1,7,12,            9,7,1,            9,8,7,            
2,4,8,            8,7,2,            2,7,12,            
10,3,2,            10,8,3,            8,7,3,            10,9,8,            
1,2,10,            8,7,3,            3,4,8,            
7,3,1,            9,7,1,            9,8,7,            
3,4,8,            8,7,3,            
11,4,3,            12,10,9,            12,9,7,            9,8,7,            
11,8,7,            12,10,3,            1,3,10,            
11,8,7,            2,3,12,            9,4,1,            
11,8,7,            2,3,12,            
11,8,7,            10,9,2,            9,4,2,            
11,8,7,            1,2,10,            
11,8,7,            9,4,1,            
11,8,7,            
7,8,11,            
7,8,11,            1,4,9,            
7,8,11,            10,2,1,            
7,8,11,            2,9,10,            2,4,9,            
7,8,11,            12,3,2,            
7,8,11,            12,3,2,            1,4,9,            
7,8,11,            3,10,12,            10,3,1,            
7,8,11,            9,10,12,            4,9,12,            3,4,12,            
3,7,8,            8,4,3,            
1,3,7,            1,7,9,            7,8,9,            
10,2,1,            3,7,8,            8,4,3,            
7,8,9,            2,7,9,            2,3,7,            10,9,2,            
8,4,2,            2,7,8,            12,7,2,            
1,2,12,            12,9,1,            7,9,12,            7,8,9,            
7,8,12,            12,8,1,            1,10,12,            8,4,1,            
9,10,12,            7,9,12,            7,8,9,            
9,7,5,            7,9,11,            
7,5,1,            11,7,1,            1,4,11,            
10,2,1,            9,7,5,            7,9,11,            
11,7,4,            4,7,10,            2,4,10,            10,7,5,            
12,3,2,            9,7,5,            7,9,11,            
12,3,2,            7,5,1,            11,7,1,            1,4,11,            
11,7,5,            5,9,11,            3,10,12,            10,3,1,            
3,4,11,            5,10,12,            12,7,5,            
7,5,3,            5,4,3,            4,5,9,            
1,3,5,            7,5,3,            
10,2,1,            7,5,3,            3,4,5,            4,5,9,            
7,5,3,            3,5,10,            2,3,10,            
4,5,9,            12,5,4,            12,7,5,            12,4,2,            
7,5,1,            1,2,7,            12,7,2,            
9,4,1,            12,7,5,            5,10,12,            
5,10,12,            12,7,5,            
7,8,11,            5,6,10,            
7,8,11,            5,6,10,            1,4,9,            
7,8,11,            6,2,1,            1,5,6,            
7,8,11,            2,4,6,            4,5,6,            9,5,4,            
12,3,2,            7,8,11,            5,6,10,            
12,3,2,            1,4,9,            5,6,10,            7,8,11,            
7,8,11,            5,3,1,            3,5,12,            5,6,12,            
12,7,6,            5,8,9,            3,4,11,            
5,6,10,            3,7,8,            8,4,3,            
5,6,10,            1,3,7,            1,7,9,            7,8,9,            
2,5,6,            5,2,1,            3,7,8,            8,4,3,            
5,8,9,            2,3,7,            7,6,2,            
5,6,10,            8,4,2,            2,7,8,            2,7,12,            
5,8,9,            12,7,6,            10,2,1,            
12,7,6,            4,5,8,            5,4,1,            
12,7,6,            5,8,9,            
11,10,9,            6,10,11,            11,7,6,            
11,7,4,            1,4,7,            7,6,1,            1,6,10,            
11,7,6,            11,6,1,            1,9,11,            6,2,1,            
2,4,6,            11,6,4,            11,7,6,            
12,3,2,            11,10,9,            6,10,11,            11,7,6,            
12,7,6,            3,4,11,            1,2,10,            
12,7,6,            11,3,1,            1,9,11,            
12,7,6,            3,4,11,            
9,4,3,            3,6,9,            10,9,6,            7,6,3,            
1,3,7,            7,6,1,            1,6,10,            
9,4,1,            2,3,7,            7,6,2,            
2,3,7,            7,6,2,            
12,7,6,            9,4,2,            10,9,2,            
1,2,10,            12,7,6,            
9,4,1,            12,7,6,            
12,7,6,            
6,8,11,            6,11,12,            
1,4,9,            6,8,11,            6,11,12,            
10,2,1,            6,8,11,            6,11,12,            
8,11,12,            6,8,12,            2,4,9,            2,9,10,            
2,6,8,            8,3,2,            3,8,11,            
1,4,9,            2,6,8,            2,3,8,            3,8,11,            
3,8,11,            10,8,3,            6,8,10,            10,3,1,            
3,4,11,            8,9,10,            6,8,10,            
4,6,8,            12,6,4,            12,4,3,            
9,8,1,            1,8,12,            6,8,12,            1,3,12,            
10,2,1,            4,6,8,            4,6,12,            12,4,3,            
2,3,12,            8,9,10,            6,8,10,            
4,6,8,            6,4,2,            
2,6,8,            2,8,9,            1,2,9,            
4,6,8,            6,4,1,            10,6,1,            
6,8,10,            8,9,10,            
9,11,12,            5,9,12,            12,6,5,            
12,6,5,            12,5,4,            4,11,12,            1,4,5,            
10,2,1,            9,11,12,            5,9,12,            12,6,5,            
10,6,5,            2,4,12,            4,11,12,            
11,3,2,            2,5,11,            5,9,11,            6,5,2,            
3,4,11,            6,5,1,            1,2,6,            
10,6,5,            1,9,11,            11,3,1,            
3,4,11,            10,6,5,            
12,6,3,            6,4,3,            6,5,4,            4,5,9,            
1,3,5,            12,5,3,            12,6,5,            
2,3,12,            10,6,5,            9,4,1,            
2,3,12,            10,6,5,            
6,4,2,            6,5,4,            4,5,9,            
6,5,1,            1,2,6,            
10,6,5,            9,4,1,            
10,6,5,            
10,11,12,            11,10,8,            5,8,10,            
1,4,9,            10,11,12,            11,10,8,            5,8,10,            
12,2,1,            1,8,12,            8,11,12,            1,5,8,            
5,8,9,            2,4,12,            4,11,12,            
11,3,2,            2,8,11,            10,8,2,            5,8,10,            
5,8,9,            1,2,10,            3,4,11,            
5,3,1,            3,5,8,            3,8,11,            
5,8,9,            3,4,11,            
12,4,3,            12,5,4,            4,5,8,            12,10,5,            
5,8,9,            1,3,10,            12,10,3,            
2,3,12,            5,4,1,            4,5,8,            
2,3,12,            5,8,9,            
8,4,2,            10,8,2,            5,8,10,            
1,2,10,            5,8,9,            
5,4,1,            4,5,8,            
5,8,9,            
9,11,12,            12,10,9,            
10,11,12,            11,10,1,            1,4,11,            
9,11,12,            2,9,12,            9,2,1,            
4,11,12,            2,4,12,            
11,10,9,            3,10,11,            10,3,2,            
3,4,11,            1,2,10,            
11,3,1,            1,9,11,            
3,4,11,            
12,10,9,            12,9,4,            12,4,3,            
1,3,10,            12,10,3,            
2,3,12,            9,4,1,            
2,3,12,            
10,9,2,            9,4,2,            
1,2,10,            
 9,4,1};
