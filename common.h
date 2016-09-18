/*
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#define OFFMAP '-'               /* space character                        */
#define SPACE '.'               /* space character                         */
#define NEWCO '+'               /* new company character                   */
#define STAR '*'                /* star character                          */
#define BLACKHOLE '@'           /* black hole character                    */

#define NUMCO 5                 /* number of companies (don't change)      */
#define INIT_CO_COST 100        /* initial company start cost              */
#define FOUNDER_BONUS 5         /* founder gets this much stock            */
#define NUMMOVES 5              /* number of moves options a player gets   */
#define MAXPLAYERS 4            /* total number of players a game can have */
#define STARCOST 500            /* company's price increase near star      */
#define BLACKHOLECOST -500      /* price increase near black hole          */
#define NEWCOCOST 100           /* company's price increase near new co    */


#define Mx 12                     /* x dimension of map */
#define My 10                     /* y dimension of map */

/* macros to look at surrounding spaces on the map: */


#define iscompany(c) ((c)>='A'&&(c)<='Z')
#define ripe(c) ((c)==STAR||(c)==NEWCO)
#define co_near(move) (iscompany(up_obj(move))||iscompany(down_obj(move))||iscompany(left_obj(move))||iscompany(right_obj(move)))
#define s_or_bh(c) ((c)==SPACE||(c)==BLACKHOLE)





/* player and company structures: */

typedef struct {
    char name[100];
    int holdings[NUMCO];
    long int svalue;  /* stock value -- not always accurate!! */
    long int cash;
    int ishuman;
} PLAYER;

typedef struct {
    char name[100];
    int price;
    int size;
    int potential;
    int risk;  /* value related to chance of being swallowed by Black Hole */
    int netpot; /* potential - risk */
} COMPANY;




