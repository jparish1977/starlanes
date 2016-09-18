/*  Artificial Inteligence -anything in relation to non human players */

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"



/*
int move;
char map[120]; */


/* 
**void generate_nme(nme)
**used in place of getting name from humans 
*/
#define up_obj(move) (((move)-MAPX < 0)?OFFMAP:map[(move)-MAPX])
#define down_obj(move) (((move)+MAPX >= MAPX*MAPY)?OFFMAP:map[(move)+MAPX])
#define left_obj(move) (((move)%MAPX)?map[(move)-1]:OFFMAP)
#define right_obj(move) (((move)%MAPX == MAPX-1)?OFFMAP:map[(move)+1])

void generate_nme(char *nme) 
{ 
    static int nmeNmr=0;

    switch(nmeNmr) {
        case 0: strcpy(nme, "Vorg");break;
        case 1: strcpy(nme, "Zhara");break;
        case 2: strcpy(nme, "Dushan");break;
        case 3: strcpy(nme, "Peko Pekovich");break;
        case 4: strcpy(nme, "Agram Anastija");break;
        default: strcpy(nme, "Man with no name");break;
        };
   nmeNmr++;
}

enum compass { North,East ,West,South };

enum farcompass {NorthW , FNorth, NorthE, FEast ,SouthE ,FSouth, SouthW ,FWest};

enum investstatus { none,small ,signif,main };

/* getfarmove gets the charecter value of a location 2 steps away.
** It makes no distiction between an empty space and an off map
** location 
*/
char getfarmove(enum farcompass fcom, char *map, int loc)
{
    static int MAPX=Mx ;
    static int MAPY=My;
    char ch;
    switch(fcom) {
        case NorthW:   if ((loc-1*MAPX )< 0) ch=SPACE; 
                       else {
                           ch=left_obj(loc-1*MAPX);
                       }
                       break;
        case FNorth: if ((loc-2*MAPX )< 0) ch=SPACE; else ch=map[loc-2*MAPX];break;
        case NorthE:   if ((loc-1*MAPX )< 0) ch=SPACE; 
                       else {
                           ch=right_obj(loc-1*MAPX);
                       }
                       break;
        case FEast:    if ((loc%MAPX)!=((loc+2)%MAPX)) ch=map[loc+2];
                       else ch=SPACE ;
                       break;
        case SouthE:   if ((loc+MAPX )> (MAPX*MAPY)) ch=SPACE; 
                       else {
                           ch=right_obj(loc+1*MAPX);
                       }
        case FSouth:   if ((loc+2*MAPX) > (MAPX*MAPY)) ch=SPACE; else ch=map[loc-2*MAPX];break;
/*(((loc)+2*MAPX > MAPX*MAPY)?SPACE:map[(loc)+2*MAPX]);break;*/
        case SouthW:  if ((loc+MAPX )> (MAPX*MAPY)) ch=SPACE; 
                       else {
                           ch=left_obj(loc+1*MAPX);
                       }
        case FWest:    if ((loc%MAPX)!=((loc-2)%MAPX)) ch=map[loc-2];
                       else ch=SPACE ;
                       break; 
/*if ((loc%MAPX)!=(loc-2)%MAPX))) ch=map[(loc)-2];else ch=SPACE;break;*/
        default: ch= '?';break;
        };
    return ch;
}

enum compass revdir(enum compass dir)
{
    enum compass rev;
    switch(dir) {
        case North:rev=South; break; 
        case East:rev=West; break; 
        case West:rev=East; break; 
        case South:rev=North; break; 
    }
return rev;
}    

int onestep(enum compass dir, int loc)
{
    int newloc;
    switch(dir) {
        case North:if ((loc)-Mx < 0) newloc=-1;
                   else newloc=loc-Mx;
                   break; 
        case South:if ((loc)+Mx > Mx*My) newloc=-1;
                   else newloc=loc+Mx;
                   break;      
        case West: if ((loc)%Mx ==0)newloc=-1;
                   else newloc=loc-1;
                   break; 
        case East: if ((loc)%Mx ==Mx-1)newloc=-1;
                   else newloc=loc+1;
                   break; 
    }
    return newloc;
}


/*
** placemove( move)
** place move is passed the location of the numbers
** that appear on the screen to humans and selects one of these
** locations for the computer players move
*/

char placemove(PLAYER *pla,int *move, COMPANY *co, int turn,char *map, int Difficulty)
{  
    static int MAPX=Mx ,MAPY=My;
    char  mvch; long  moveval[NUMMOVES]; int neighb[4];int nc, ncom;
    enum compass dir;
    enum investstatus curst[NUMCO];
    double coholdings[NUMCO]; double tothold[NUMCO];int curpl;
    char locs[South];
    int seed,intchar ,mvop,curCo,curMax,
    smallC,bigC; /*this is for the implications of each move */
    long bgain,sgain; /* gains in mergers */

    mvch= '1';
    curMax=-1000;
    if ((Difficulty>1 )||(rand()%100>50))
    for (curCo=0;curCo<=NUMCO;curCo++){ 
        tothold[curCo] =0; 
    /*finds the proportion of holdings relativ to other players*/
        for (curpl=0;curpl<MAXPLAYERS;curpl++) 
            tothold[curCo]=pla[curpl].holdings[curCo] +tothold[curCo];
        if (tothold[curCo]==0) coholdings[curCo]=0;
        else coholdings[curCo]=pla[turn].holdings[curCo]/tothold[curCo];
    }
    for (curCo=0;curCo<=NUMCO;curCo++){
        if (pla[turn].holdings[curCo]>0) curst[curCo]=signif;
        else curst[curCo]=none;
    }
    if ((Difficulty>1 )||(rand()%100>50))
    for (mvop=0;mvop<=4;mvop++){
        seed=0;
        locs[North] = up_obj(move[mvop]);
        locs[South] = down_obj(move[mvop]);
        locs[West]  = left_obj(move[mvop]);
        locs[East]  = right_obj(move[mvop]);
        moveval[mvop]=NEWCOCOST;
        ncom=0;
        for(dir=North;dir<=South;dir++){
            if (locs[dir]== STAR) moveval[mvop]=moveval[mvop]+STARCOST ;
            if (locs[dir]== BLACKHOLE) moveval[mvop]=moveval[mvop]+BLACKHOLECOST ;
            if (locs[dir]== NEWCO) {
                moveval[mvop]=moveval[mvop]+NEWCOCOST ;                
            }  
            if iscompany(locs[dir]) {
                nc=locs[dir]-'A';
                if ((ncom==0)||(nc!=neighb[1])){
                    ncom++;   /* todo add for cases when 3 co ajacent*/
                    neighb[ncom]= nc;
                }
            }
        }
        if (ncom==0) if (moveval[mvop]>100) moveval[mvop]*=FOUNDER_BONUS; else moveval[mvop]=0 ;
    /*need to check here whether company eliminated by black hole */
        else if (ncom==1){ 
            /*if company next door and holding is minimal then value will be reversed*/
            moveval[mvop]=moveval[mvop]*( coholdings[neighb[1]]-0.25)*tothold[ncom];      
        }
        else if (ncom>1){ 
          /*calculate value of merger*/
            smallC=1; 
            bigC=1;
            for (curCo=2;curCo<=ncom;curCo++){
                if (co[neighb[curCo]].price<co[neighb[smallC]].price)
                     smallC= curCo;
                if (co[neighb[curCo]].price>co[neighb[bigC]].price)
                     bigC= curCo;    
            } 
            sgain=co[neighb[smallC]].price*tothold[neighb[smallC]]*
                        (coholdings[neighb[smallC]]-0.25)/2;/*bonus*/
            sgain+=co[bigC].price*tothold[neighb[smallC]]*
                        (coholdings[neighb[smallC]]-0.25);
            bgain=(moveval[mvop]+co[neighb[smallC]].price)*
                        tothold[neighb[bigC]]*(coholdings[neighb[bigC]]-0.25);
            moveval[mvop]=sgain+bgain;
 
        }
        if (moveval[mvop]>curMax) {
            intchar= mvop +(int)'1';
            mvch= (char) intchar;
            curMax=moveval[mvop] ;
        }
    }
    else mvch=1;
    return mvch;
}


/* 
** distantblackhole starts at the black hole, moves two away and then 
** looks at the three ajacent points for companies.  It then increases
** the risk factor for any companies found.  It isn't
** always able to detect when the route between the black hole
** and company is blocked so sometimes sees a risk that isn't there.
*/

void distantblackhole(COMPANY *co,char *map,int loc)
{
    int step,cloc;
    enum compass dir,end;
    int  neighb[4], ncom, new,isnew,curCo;
    
    for (dir=North;dir<=South;dir++){
        step =onestep(dir, loc);
        if (step>-1) if (map[step]==SPACE){
            step =onestep(dir, step);
            ncom=0;
            if (step>-1) if (!(iscompany(map[step]))) for (end=North;end<=South;end++)
                if (end!=revdir(dir)){    /*point ajacent bh checked elsewhere*/
                    cloc= onestep(end, step);
                    if iscompany(map[cloc]) {
                        new=map[cloc]-'A';
                        isnew=1; 
                        curCo=0;
                        while ((curCo<=ncom) && (isnew==1)){
                            if (curCo==ncom)  /* no need to check if co already found if first */
                                neighb[curCo]=new;
                            else if (neighb[curCo]==new) isnew=0;
                            curCo++;
                        }
                    if (isnew==1)  {
                        ncom++;
                        neighb[curCo]=new;
                    }
                    }
                }
            curCo=1;
            while (curCo<=ncom){ 
                co[neighb[curCo]].risk+=  BLACKHOLECOST/2; 
                curCo++;
            }
        }
    }     
}

/* 
**ai_buy_sell not only choses the most promising investments but
**actually executes the sales 
*/

void ai_buy_sell(PLAYER *pl, COMPANY *co, int turn,char *map, int Difficulty)
{
    int curloc,curCo,BuyCo,SecCo,amt ; 
    static int MAPX=Mx , MAPY=My;
    int localpt, localris;
    enum compass dir; 
    int  neighb[4];
    int nc, ncom,smallC,bigC; 
    char locs[South];

    for(curCo=0;curCo<NUMCO;curCo++) {        /*reset values*/
        co[curCo].potential=co[curCo].risk=0;
    } 
    for (curloc=1;curloc<=(Mx*My);curloc++){
        if (map[curloc]==BLACKHOLE) distantblackhole(co,map,curloc);
        if (map[curloc]==SPACE);{
            locs[North] = up_obj(curloc); /*get the chars of the 4 ajacent points*/
            locs[South] = down_obj(curloc);
            locs[West]  = left_obj(curloc);
            locs[East]  = right_obj(curloc);
            localris=0; 
            localpt=NEWCOCOST; /*the gain from the point itself */
            for(dir=North;dir<=South;dir++){   
                if (locs[dir]== STAR) localpt=localpt+STARCOST ;/*First check the value or */
                if ((Difficulty>2 )||(rand()%100>50))
                    if (locs[dir]== BLACKHOLE) 
                            localris=localris+BLACKHOLECOST;/*loss from the point*/
                if (locs[dir]== NEWCO) localpt=localpt+NEWCOCOST ;
            }
        }
        ncom=0;
        for(dir=North;dir<=South;dir++){  /*Then add those values to the potetial of companies*/
            if (iscompany(locs[dir])) {  /*that are in the vicinity */
            nc=locs[dir]-'A';
                if ((ncom==0)||(nc!=neighb[1])){
                    ncom++;   /* todo add for cases when 3 co ajacent*/
                    neighb[ncom]= nc;
                }
            }
        }
        if (ncom==1 ){                
                co[nc].potential=co[nc].potential+localpt;
                co[nc].risk=co[nc].risk+localris; 
        }
        else if (ncom>1){  
            smallC=1; 
            bigC=1;
            for (curCo=2;curCo<=ncom;curCo++){
                if (co[neighb[curCo]].price<co[neighb[smallC]].price)
                     smallC= curCo;
                if (co[neighb[curCo]].price>co[neighb[bigC]].price)
                     bigC= curCo;    
            } 
            /* bonus ignored for now*/
            co[neighb[bigC]].potential=co[neighb[bigC]].potential+(co[neighb[smallC]].price);
            co[neighb[smallC]].potential=co[neighb[smallC]].potential+co[neighb[bigC]].price/2;
        }
    }
    
    SecCo=BuyCo=-1 ;
    for(curCo=0;curCo<NUMCO;curCo++) /*Find the co with greatest potential*/
        if (co[curCo].price>0) {
            co[curCo].netpot=(co[curCo].potential+co[curCo].risk)/
                       (co[curCo].price); 
            if ((co[curCo].netpot>0)){
                if (BuyCo==-1)   BuyCo=curCo;
                else {
                    if (co[curCo].netpot>co[BuyCo].netpot) {
                        SecCo=BuyCo;  
                        BuyCo=curCo;
                    }
                    else {
                        if (SecCo==-1) SecCo=curCo;
                        else if (co[curCo].netpot>co[SecCo].netpot) 
                            SecCo=curCo; 
                    }
                }              
            }
        }
    if ((Difficulty>2 )||((Difficulty>1 )&&(rand()%100>50)) )
        for(curCo=0;curCo<NUMCO;curCo++){  /*sell all*/
            pl[turn].cash += pl[turn].holdings[curCo]*co[curCo].price;
            pl[turn].holdings[curCo]=0;
        } 
    if (BuyCo>-1){ /*now buy shares in the company with the best prospects*/
        if (co[BuyCo].risk<0) { /*don't put all your eggs into one basket*/
            amt= pl[turn].cash/(co[BuyCo].price*2);
            pl[turn].cash += (-amt * co[BuyCo].price);
            pl[turn].holdings[BuyCo] += amt;
            if (SecCo>-1){             
                amt= pl[turn].cash/co[SecCo].price;
                pl[turn].cash += (-amt * co[SecCo].price);
                pl[turn].holdings[SecCo] += amt;
            }
        }
        else  {
            amt= pl[turn].cash/co[BuyCo].price; 
            pl[turn].cash += (-amt * co[BuyCo].price);
            pl[turn].holdings[BuyCo] += amt;
        }
    }
}
