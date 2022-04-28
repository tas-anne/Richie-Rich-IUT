#define _CRT_SECURE_NO_WARNINGS

#include "Player.h"
#include "Game.h"
#include "Property.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

typedef enum
{
    Game_STATE_BEGIN_MOVE,
    Game_STATE_BUY_PROPERTY,
    Game_STATE_PLAYER_IN_DEBT,
    Game_STATE_END
} Game_State;

void Game_buyProperty();
static void Game_goBankrupt();
static const int TOTAL_PLAYERS = 2;
static int cur_player = 0;
static student Game_players[2];
static Game_Property gProperties[40];
static Game_State gState;
static int lastRoll[2];
static int selectedProperty;
static char * message;
static char * message2;

char* Game_getFormattedStatus(int player)
{
    char* buffer = (char *)malloc(sizeof(char) * 20);
    if (cur_player == player)
        sprintf(buffer,"> %s %dTK %.2f %d",Game_players[player].name, Game_players[player].allowance, Game_players[player].CGPA, Game_players[player].free_pass);
    else
        sprintf(buffer,"%s %dTK %.2f %d",Game_players[player].name, Game_players[player].allowance, Game_players[player].CGPA, Game_players[player].free_pass);
    return buffer;
}

static void updateMessage2(char * newMessage)
{
    free(message2);
    message2 = newMessage;
}

static void Game_property_init()
{
    int i;
    for (i = 0; i < 40; i++)
    {
        gProperties[i].id = i;
        gProperties[i].mortaged = 0;
        gProperties[i].owner = -1;
        gProperties[i].upgrades = 0;
        gProperties[i].price = 0;
    }

    gProperties[0].type = G_type_ABBANK;

    gProperties[1].type = G_type_BROWN;
    gProperties[1].price = 600;
    gProperties[1].rentCost[0] = 20;
    gProperties[1].rentCost[1] = 100;
    gProperties[1].rentCost[2] = 300;
    gProperties[1].rentCost[3] = 900;
    gProperties[1].rentCost[4] = 1600;
    gProperties[1].rentCost[5] = 2500;
    gProperties[1].upgradeCost = 500;

    gProperties[2].type = G_type_TASK1;
    gProperties[7].type = G_type_TASK2;
    gProperties[17].type = G_type_TASK3;
    gProperties[22].type = G_type_TASK4;
    gProperties[33].type = G_type_TASK5;
    gProperties[36].type = G_type_TASK6;

    gProperties[3].type = G_type_BROWN;
    gProperties[3].price = 800;
    gProperties[3].rentCost[0] = 40;
    gProperties[3].rentCost[1] = 200;
    gProperties[3].rentCost[2] = 600;
    gProperties[3].rentCost[3] = 1800;
    gProperties[3].rentCost[4] = 3200;
    gProperties[3].rentCost[5] = 4500;
    gProperties[3].upgradeCost = 500;

    gProperties[4].type = G_type_LIBRARY_FEES;

    for (i = 5; i < 40; i+= 10)
    {
        gProperties[i].type = G_type_STATION;
        gProperties[i].price = 2000;
    }

    for (i = 6; i <= 8; i+=2)
    {
        gProperties[i].type = G_type_LIGHTBLUE;
        gProperties[i].price = 1000;
        gProperties[i].rentCost[0] = 60;
        gProperties[i].rentCost[1] = 300;
        gProperties[i].rentCost[2] = 900;
        gProperties[i].rentCost[3] = 2700;
        gProperties[i].rentCost[4] = 4000;
        gProperties[i].rentCost[5] = 5500;
        gProperties[i].upgradeCost = 500;
    }

    gProperties[9].type = G_type_LIGHTBLUE;
    gProperties[9].price = 1200;
    gProperties[9].rentCost[0] = 80;
    gProperties[9].rentCost[1] = 400;
    gProperties[9].rentCost[2] = 1000;
    gProperties[9].rentCost[3] = 3000;
    gProperties[9].rentCost[4] = 4500;
    gProperties[9].rentCost[5] = 6000;
    gProperties[9].upgradeCost = 500;

    gProperties[10].type = G_type_PUNISHMENT;

    for (i = 11; i <= 13; i+=2)
    {
        gProperties[i].type = G_type_PINK;
        gProperties[i].price = 1400;
        gProperties[i].rentCost[0] = 100;
        gProperties[i].rentCost[1] = 500;
        gProperties[i].rentCost[2] = 1500;
        gProperties[i].rentCost[3] = 4500;
        gProperties[i].rentCost[4] = 6250;
        gProperties[i].rentCost[5] = 7500;
        gProperties[i].upgradeCost = 1000;
    }

    for (i = 12; i <= 28; i+= 16)
    {
        gProperties[i].type = G_type_SERVICE;
        gProperties[i].price = 1500;
    }

    gProperties[14].type = G_type_PINK;
    gProperties[14].price = 1600;
    gProperties[14].rentCost[0] = 120;
    gProperties[14].rentCost[1] = 600;
    gProperties[14].rentCost[2] = 1800;
    gProperties[14].rentCost[3] = 5000;
    gProperties[14].rentCost[4] = 7000;
    gProperties[14].rentCost[5] = 9000;
    gProperties[14].upgradeCost = 1000;

    for (i = 16; i <= 18; i+=2)
    {
        gProperties[i].type = G_type_ORANGE;
        gProperties[i].price = 1800;
        gProperties[i].rentCost[0] = 140;
        gProperties[i].rentCost[1] = 700;
        gProperties[i].rentCost[2] = 2000;
        gProperties[i].rentCost[3] = 5500;
        gProperties[i].rentCost[4] = 7500;
        gProperties[i].rentCost[5] = 9500;
        gProperties[i].upgradeCost = 1000;
    }

    gProperties[19].type = G_type_ORANGE;
    gProperties[19].price = 2000;
    gProperties[19].rentCost[0] = 160;
    gProperties[19].rentCost[1] = 800;
    gProperties[19].rentCost[2] = 2200;
    gProperties[19].rentCost[3] = 6000;
    gProperties[19].rentCost[4] = 8000;
    gProperties[19].rentCost[5] = 10000;
    gProperties[19].upgradeCost = 1000;

    gProperties[20].type = G_type_LAKE;

    for (i = 21; i <= 23; i+=2)
    {
        gProperties[i].type = G_type_RED;
        gProperties[i].price = 2200;
        gProperties[i].rentCost[0] = 180;
        gProperties[i].rentCost[1] = 900;
        gProperties[i].rentCost[2] = 2500;
        gProperties[i].rentCost[3] = 7000;
        gProperties[i].rentCost[4] = 8750;
        gProperties[i].rentCost[5] = 10500;
        gProperties[i].upgradeCost = 1500;
    }

    gProperties[24].type = G_type_RED;
    gProperties[24].price = 2400;
    gProperties[24].rentCost[0] = 200;
    gProperties[24].rentCost[1] = 1000;
    gProperties[24].rentCost[2] = 3000;
    gProperties[24].rentCost[3] = 7500;
    gProperties[24].rentCost[4] = 9250;
    gProperties[24].rentCost[5] = 11000;
    gProperties[24].upgradeCost = 1500;

    for (i = 26; i <= 27; i+=1)
    {
        gProperties[i].type = G_type_YELLOW;
        gProperties[i].price = 2600;
        gProperties[i].rentCost[0] = 220;
        gProperties[i].rentCost[1] = 1100;
        gProperties[i].rentCost[2] = 3300;
        gProperties[i].rentCost[3] = 8000;
        gProperties[i].rentCost[4] = 9750;
        gProperties[i].rentCost[5] = 11500;
        gProperties[i].upgradeCost = 1500;
    }

    gProperties[29].type = G_type_YELLOW;
    gProperties[29].price = 2800;
    gProperties[29].rentCost[0] = 240;
    gProperties[29].rentCost[1] = 1200;
    gProperties[29].rentCost[2] = 3600;
    gProperties[29].rentCost[3] = 8500;
    gProperties[29].rentCost[4] = 10250;
    gProperties[29].rentCost[5] = 12000;
    gProperties[29].upgradeCost = 1500;

    gProperties[30].type = G_type_GOTO_VC_OFFICE;

    for (i = 31; i <= 32; i+=1)
    {
        gProperties[i].type = G_type_GREEN;
        gProperties[i].price = 3000;
        gProperties[i].rentCost[0] = 260;
        gProperties[i].rentCost[1] = 1300;
        gProperties[i].rentCost[2] = 3900;
        gProperties[i].rentCost[3] = 9000;
        gProperties[i].rentCost[4] = 11000;
        gProperties[i].rentCost[5] = 12750;
        gProperties[i].upgradeCost = 2000;
    }

    gProperties[34].type = G_type_GREEN;
    gProperties[34].price = 3200;
    gProperties[34].rentCost[0] = 280;
    gProperties[34].rentCost[1] = 1500;
    gProperties[34].rentCost[2] = 4500;
    gProperties[34].rentCost[3] = 10000;
    gProperties[34].rentCost[4] = 12000;
    gProperties[34].rentCost[5] = 14000;
    gProperties[34].upgradeCost = 2000;

    gProperties[37].type = G_type_BLUE;
    gProperties[37].price = 3500;
    gProperties[37].rentCost[0] = 350;
    gProperties[37].rentCost[1] = 1750;
    gProperties[37].rentCost[2] = 5000;
    gProperties[37].rentCost[3] = 11000;
    gProperties[37].rentCost[4] = 13000;
    gProperties[37].rentCost[5] = 15000;
    gProperties[37].upgradeCost = 2000;

    gProperties[38].type = G_type_extra_internet_usage;

    gProperties[39].type = G_type_BLUE;
    gProperties[39].price = 4000;
    gProperties[39].rentCost[0] = 500;
    gProperties[39].rentCost[1] = 2000;
    gProperties[39].rentCost[2] = 6000;
    gProperties[39].rentCost[3] = 14000;
    gProperties[39].rentCost[4] = 17000;
    gProperties[39].rentCost[5] = 20000;
    gProperties[39].upgradeCost = 2000;
}

void Game_init()
{
    int i;
    message = (char *)malloc(sizeof(char) * 40);
    message = "";
    Game_property_init();
    srand(time(NULL));
    cur_player = 0;
    for (i = 0; i < TOTAL_PLAYERS; i++)
    {
        Game_players[i].id = i;
        Game_players[i].CGPA=0.00;
        Game_players[i].allowance= 20000;
        Game_players[i].pos= 0;
        Game_players[i].punished=0;
        Game_players[i].free_pass=0;
    }
    printf("ENTER STUDENT NAMES: ");
    scanf("%s %s", &Game_players[0].name, &Game_players[1].name);
    gState = Game_STATE_BEGIN_MOVE;
}

static void nextPlayer()
{
    if (Game_STATE_PLAYER_IN_DEBT != gState)
        cur_player = (cur_player + 1) % 2;
}

static void Game_process_debts()
{
    if (Game_STATE_PLAYER_IN_DEBT != gState)
        return;
    message = "Time to go home!";
    if (Game_players[cur_player].allowance >= 0 && Game_players[cur_player].CGPA >2)
    {
        gState = Game_STATE_BEGIN_MOVE;
        nextPlayer();
        message = "";
    }
}

static void Game_pay_player(int payer, int receiever, int sum)
{
    char * buffer = (char *) malloc(sizeof(char) * 30);
    sprintf(buffer, "%s paid %s $%i", Game_players[payer].name, Game_players[receiever].name, sum);
    updateMessage2(buffer);

    Game_players[payer].allowance-= sum;
    Game_players[receiever].allowance+= sum;
    if (Game_players[payer].allowance< 0)
    {
        gState = Game_STATE_PLAYER_IN_DEBT;
        Game_process_debts();
    }

}

static void Game_remove_money(int player, int amount)
{
    char * buffer = (char *) malloc(sizeof(char) * 30);
    sprintf(buffer, "%s paid $%i", Game_players[player].name, amount);
    updateMessage2(buffer);
    Game_players[player].allowance-= amount;
    if (Game_players[player].allowance< 0)
    {
        gState = Game_STATE_PLAYER_IN_DEBT;
        Game_process_debts();
    }
}

static void Game_roll_dice()
{
    int i;
    for (i = 0; i < 2; i++)
        lastRoll[i] = rand() % 6 + 1;
}

static int pop_up()
{
    const SDL_MessageBoxButtonData buttons[]=
    {
        {/*flags,buttonid,text*/ 0,0,"Buy"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Bid"},
    };
    const SDL_MessageBoxColorScheme colorScheme =
    {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata =
    {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Current player", /* .title */
        "Bid/Buy?", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
    {
        SDL_Log("error displaying message box");
        return 1;
    }
    if (buttonid == -1)
    {
        SDL_Log("no selection");
    }
    if(buttonid==0)
    {
        gState = Game_STATE_BUY_PROPERTY;
        gState = Game_STATE_BEGIN_MOVE;
        return 19;
    }
    else if(buttonid==1)
    {
        //SDL_Window *gameWindow;
        nextPlayer();
        //gState = Game_STATE_BEGIN_MOVE;
        const SDL_MessageBoxButtonData buttons2[]=
        {
            {/*flags,buttonid,text*/ 0,0,"Buy"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Ignore"},
        };
        const SDL_MessageBoxColorScheme colorScheme2 =
        {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata2 =
        {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            NULL, /* .window */
            "Next player", /* .title */
            "Ignore/Buy?", /* .message */
            SDL_arraysize(buttons2), /* .numbuttons */
            buttons2, /* .buttons */
            &colorScheme2 /* .colorScheme */
        };
        int buttonid2;

        if (SDL_ShowMessageBox(&messageboxdata2, &buttonid2) < 0)
        {
            SDL_Log("error displaying message box");
            return 1;
        }
        if (buttonid2 == -1)
        {
            SDL_Log("no selection");
        }
        else if(buttonid2==0)
        {
            gState=Game_STATE_BUY_PROPERTY;
            gState=Game_STATE_BEGIN_MOVE;
            return 27;
        }
        else if(buttonid2==1)
        {
            nextPlayer();
            return 1;
        }
    }
    return 0;
}

static int Game_isPlayerAMonopolist(int player, G_Property_t type);

static void Game_player_land(int newposition)
{
    char * buffer = (char *) malloc(sizeof(char) * 100);
    /* if a buyable property */
    if (gProperties[newposition].price > 0 && gProperties[newposition].mortaged == 0)
    {
        if (gProperties[newposition].owner == -1)
        {
            gState = Game_STATE_BUY_PROPERTY;
        }
        else if (gProperties[newposition].owner != cur_player)
        {
            if (gProperties[newposition].type != G_type_SERVICE &&
                    gProperties[newposition].type !=G_type_STATION)
            {
                int mult = 1;
                if (Game_isPlayerAMonopolist(gProperties[newposition].owner, gProperties[newposition].type)
                        && gProperties[newposition].upgrades == 0)
                    mult = 2;
                Game_pay_player(cur_player, gProperties[newposition].owner,
                                mult * gProperties[newposition].rentCost[gProperties[newposition].upgrades]);
            }
            if (gProperties[newposition].type == G_type_STATION)
            {
                int owner = gProperties[newposition].owner;
                int i, m = 1;
                for (i = 5; i <= 35; i+=10)
                {
                    if (gProperties[i].owner == owner && gProperties[i].mortaged == 0)
                        m *= 2;
                }
                Game_pay_player(cur_player, owner, 25 * m / 2);
            }
            if (gProperties[newposition].type == G_type_SERVICE)
            {
                int owner = gProperties[newposition].owner;
                int i, m = 0;
                for (i = 12; i <= 38; i+=16)
                {
                    if (gProperties[i].owner == owner && gProperties[i].mortaged == 0)
                        m += 1;
                }

                i = (1 == m ? 4 : 10) * (lastRoll[0] + lastRoll[1]);
                Game_pay_player(cur_player, owner, i);
            }

        }
    }
    else
    {
        switch (gProperties[newposition].type)
        {
        case G_type_ABBANK:
            break;
        case G_type_LIBRARY_FEES:
            Game_remove_money(cur_player, 2000);
            break;
        case G_type_extra_internet_usage:
            Game_remove_money(cur_player, 1000);
            break;
        case G_type_TASK1:
            Game_players[cur_player].pos+=3;
            sprintf(buffer, "Task: Go 3 step forward.");
            updateMessage2(buffer);
            if(gProperties[5].owner==-1)
                Game_buyProperty();
            else
                Game_player_land(5);
            break;
        case G_type_TASK2:
            Game_players[cur_player].allowance-=100;
            Game_players[(cur_player+1)%2].allowance+=100;
            sprintf(buffer, "Task: give birthday gift.");
            updateMessage2(buffer);
            break;
        case G_type_TASK3:
            Game_players[cur_player].pos+=8;
            sprintf(buffer, "Task: Go to the nearest station.");
            updateMessage2(buffer);
            if(gProperties[5].owner==-1)
                Game_buyProperty();
            else
                Game_player_land(25);
            break;
        case G_type_TASK4:
            Game_players[cur_player].free_pass++;
            sprintf(buffer, "Task: Got free pass.");
            updateMessage2(buffer);
            break;
        case G_type_TASK5:
            Game_players[cur_player].pos-=3;
            sprintf(buffer, "Task: Go back 3 step.");
            updateMessage2(buffer);
            break;
        case G_type_TASK6:
            Game_players[cur_player].allowance-=200;
            sprintf(buffer, "Task: Penalty: Broke Sunset law.");
            updateMessage2(buffer);
            break;
        default:
            break;
        }
    }
}

static void VC_office()
{
    Game_players[cur_player].pos=10;
    Game_players[cur_player].punished=1;
}

static void pop_up2()
{

    if(Game_players[cur_player].free_pass>0)
    {
        const SDL_MessageBoxButtonData buttons[]=
        {
            {/*flags,buttonid,text*/ 0,0,"Pay money"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Matching dice"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,2,"Use Free pass"},
        };

        const SDL_MessageBoxColorScheme colorScheme =
        {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata =
        {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            NULL, /* .window */
            "~Get out of VC's office~", /* .title */
            "What's your choice?", /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
        };
        int buttonid;
        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
        {
            SDL_Log("error displaying message box");
            return;
        }
        if (buttonid == -1)
        {
            SDL_Log("no selection");
        }
        if(buttonid==0)
        {
            Game_players[cur_player].punished=0;
            Game_players[cur_player].dice_rolled=0;
            Game_players[cur_player].allowance-=500;
        }
        else if(buttonid==1)
        {
            if(lastRoll[0]==lastRoll[1])
            {
                Game_players[cur_player].punished=0;
                Game_players[cur_player].dice_rolled=0;
            }
        }
        else if(buttonid==2)
        {
            Game_players[cur_player].punished=0;
            Game_players[cur_player].dice_rolled=0;
            Game_players[cur_player].free_pass--;
        }
    }
    else
    {
        const SDL_MessageBoxButtonData buttons[]=
        {
            {/*flags,buttonid,text*/ 0,0,"Pay money"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Matching dice"},
        };

        const SDL_MessageBoxColorScheme colorScheme =
        {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata =
        {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            NULL, /* .window */
            "~Get out of VC's office~", /* .title */
            "What's your choice?", /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
        };
        int buttonid;
        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
        {
            SDL_Log("error displaying message box");
            return;
        }
        if (buttonid == -1)
        {
            SDL_Log("no selection");
        }
        if(buttonid==0)
        {
            Game_players[cur_player].punished=0;
            Game_players[cur_player].dice_rolled=0;
            Game_players[cur_player].allowance-=500;
        }
        else if(buttonid==1)
        {
            if(lastRoll[0]==lastRoll[1])
            {
                Game_players[cur_player].punished=0;
                Game_players[cur_player].dice_rolled=0;
            }
        }
    }
}

void Game_cycle()
{
    int roll, count=0;
    if(Game_players[cur_player].dice_rolled==3)
    {
        Game_players[cur_player].punished=0;
    }
    if(Game_players[cur_player].punished==1)
    {
        pop_up2();
        if(Game_players[cur_player].CGPA>0)
        {
            Game_players[cur_player].CGPA-=.5;
            if(Game_players[cur_player].CGPA==2)
            {
                Game_goBankrupt();
            }

        }
        Game_players[cur_player].dice_rolled++;

    }
    Game_roll_dice();
    roll = lastRoll[0] + lastRoll[1];
    if (Game_players[cur_player].pos+ roll >= 40)
    {
        Game_players[cur_player].allowance+= 2000;
        Game_players[cur_player].CGPA=4.00;
    }
    if(Game_players[cur_player].punished==0)
    {
        Game_players[cur_player].pos = (Game_players[cur_player].pos + roll) % 40;
        Game_player_land(Game_players[cur_player].pos);
        if(lastRoll[0]==lastRoll[1] && count<3)
        {
            count++;
        }
        if((lastRoll[0]==lastRoll[1] && count==3) || Game_players[cur_player].pos==30)
        {
            char * buffer = (char *) malloc(sizeof(char) * 30);
            sprintf(buffer, "%s goes to VC's office.", Game_players[cur_player].name);
            updateMessage2(buffer);
            VC_office();
        }
    }
    if (gState == Game_STATE_BEGIN_MOVE && lastRoll[0] != lastRoll[1])
        nextPlayer();
}


void Game_buyProperty()
{
    int newposition = Game_getPlayerPosition(cur_player);
    if (gProperties[newposition].price > 0)
    {
        if (gProperties[newposition].owner == -1)
        {
            gState = Game_STATE_BEGIN_MOVE;
            int x = pop_up();
            if(x==19)
            {
                Game_remove_money(cur_player, gProperties[newposition].price);
                gProperties[newposition].owner = cur_player;
            }
            if(x==27)
            {
                Game_remove_money(cur_player, gProperties[newposition].price);
                gProperties[newposition].owner = cur_player;
                nextPlayer();
            }
        }
    }
}

int Game_getTotalPlayers()
{
    return TOTAL_PLAYERS;
}

int Game_getPlayerPosition(int playerid)
{
    return Game_players[playerid].pos;
}

int Game_getPropOwner(int propid)
{
    if (gProperties[propid].price > 0 )
    {
        return gProperties[propid].owner;
    }
    else
        return -1;
}

static void Game_mortageProp(int propid)
{
    if (gProperties[propid].owner == cur_player)
    {
        int i;
        if (gProperties[propid].upgrades > 0)
        {
            message = "Can't mortgage with upgrades";
            return;
        }
        for (i = 0; i < 40; i++)
        {
            if (gProperties[i].type == gProperties[propid].type)
            {
                if (gProperties[i].upgrades > 0)
                {
                    message = "Destroy upgrades first";
                    return;
                }
            }
        }

        if (gProperties[propid].mortaged == 0 && gProperties[propid].upgrades == 0)
        {
            Game_players[cur_player].allowance+= gProperties[propid].price / 2;
            gProperties[propid].mortaged = 1;
        }
        else
        {
            if (Game_players[cur_player].allowance>= gProperties[propid].price / 2 )
            {
                gProperties[propid].mortaged = 0;
                Game_remove_money(cur_player, (int)(gProperties[propid].price / 2));
            }
        }
    }
    else
    {
        message = "Can't mortgage that";
    }
}

static int Game_isPlayerAMonopolist(int player, G_Property_t type)
{
    int i;
    if (G_type_SERVICE == type || G_type_STATION == type)
        return 0;

    for (i = 0; i < 40; i++)
    {
        if (gProperties[i].type == type)
            if (gProperties[i].owner != player)
            {
                return 0;
            }
    }
    return 1;
}

static int Game_isLegitUpgrade(int player, int propid)
{
    int highest_upgrade = 0;
    int lowest_upgrade = 5;
    int i;
    int this_upgrade = gProperties[propid].upgrades;
    G_Property_t p_type = gProperties[propid].type;

    if (Game_isPlayerAMonopolist(player, p_type))
    {
        for (i = 0; i < 40; i++)
        {
            if (gProperties[i].type == p_type)
            {
                if (gProperties[i].mortaged == 1)
                    return 0;
                highest_upgrade = gProperties[i].upgrades > highest_upgrade ?
                                  gProperties[i].upgrades : highest_upgrade;
                lowest_upgrade = gProperties[i].upgrades < lowest_upgrade ?
                                 gProperties[i].upgrades : lowest_upgrade;
            }
        }
    }

    if (this_upgrade == lowest_upgrade)
        return 1;
    if (highest_upgrade == lowest_upgrade)
        return 1;
    return 0;
}

static int Game_isLegitDowngrade(int player, int propid)
{
    int highest_upgrade = 0;
    int lowest_upgrade = 5;
    int i;
    int this_upgrade = gProperties[propid].upgrades;
    G_Property_t p_type = gProperties[propid].type;

    if (Game_isPlayerAMonopolist(player, p_type))
    {
        for (i = 0; i < 40; i++)
        {
            if (gProperties[i].type == p_type)
            {
                if (gProperties[i].mortaged == 1)
                    return 0;
                highest_upgrade = gProperties[i].upgrades > highest_upgrade ?
                                  gProperties[i].upgrades : highest_upgrade;
                lowest_upgrade = gProperties[i].upgrades < lowest_upgrade ?
                                 gProperties[i].upgrades : lowest_upgrade;
            }
        }
    }

    if (this_upgrade == highest_upgrade)
        return 1;
    if (highest_upgrade == lowest_upgrade)
        return 1;
    return 0;
}

static void pop_up4(int propid)
{
    const SDL_MessageBoxButtonData buttons[]=
    {
        {/*flags,buttonid,text*/ 0,0,"Be Evil"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Be Happy"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,2,"Be Rich"},
    };

    const SDL_MessageBoxColorScheme colorScheme =
    {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata =
    {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "~Man! You nailed it!!~", /* .title */
        "Follow your instincts:", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
    {
        SDL_Log("error displaying message box");
        return;
    }
    if (buttonid == -1)
    {
        SDL_Log("no selection");
    }
    if(buttonid==0)
    {
        const SDL_MessageBoxButtonData buttons4[]=
        {

            //{/*flags,buttonid,text*/ 0,0,"continue"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"continue"},


        };
        const SDL_MessageBoxColorScheme colorScheme4 =
        {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata4 =
        {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            NULL, /* .window */
            "Rewarded!! You chose to be evil!!^_^", /* .title */
            "Choose any property owned by opponent that has no upgrades!",/*message*/
            SDL_arraysize(buttons4), /* .numbuttons */
            buttons4, /* .buttons */
            &colorScheme4 /* .colorScheme */
        };
        int buttonid4;
        if (SDL_ShowMessageBox(&messageboxdata4, &buttonid4) < 0)
        {
            SDL_Log("error displaying message box");
            return;
        }
        if(gProperties[propid].owner==(cur_player+1)%2 && gProperties[propid].upgrades==0)
        {
            gProperties[propid].rentCost[0]=gProperties[propid].rentCost[0]/2;
        }
        return;
    }
    if(buttonid==1)
    {
        const SDL_MessageBoxButtonData buttons4[]=
        {

            //{/*flags,buttonid,text*/ 0,0,"continue"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"continue"},


        };
        const SDL_MessageBoxColorScheme colorScheme4 =
        {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata4 =
        {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            NULL, /* .window */
            "Rewarded!! You chose to be happy!!^_^", /* .title */
            "Choose any property owned by yourself, that has no upgrades!",/*message*/
            SDL_arraysize(buttons4), /* .numbuttons */
            buttons4, /* .buttons */
            &colorScheme4 /* .colorScheme */
        };
        int buttonid4;
        if (SDL_ShowMessageBox(&messageboxdata4, &buttonid4) < 0)
        {
            SDL_Log("error displaying message box");
            return;
        }
        if(gProperties[propid].owner==cur_player && gProperties[propid].upgrades==0)
        {
            gProperties[propid].rentCost[0]+=gProperties[propid].rentCost[0]/2;
        }
        return;
    }
    if(buttonid==2)
    {
        const SDL_MessageBoxButtonData buttons4[]=
        {

            //{/*flags,buttonid,text*/ 0,0,"continue"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"continue"},


        };
        const SDL_MessageBoxColorScheme colorScheme4 =
        {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata4 =
        {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            NULL, /* .window */
            "Rewarded!! You chose to be rich!!$_$", /* .title */
            "Congrats! You played safe!! :v",/*message*/
            SDL_arraysize(buttons4), /* .numbuttons */
            buttons4, /* .buttons */
            &colorScheme4 /* .colorScheme */
        };
        int buttonid4;
        if (SDL_ShowMessageBox(&messageboxdata4, &buttonid4) < 0)
        {
            SDL_Log("error displaying message box");
            return;
        }
        Game_players[cur_player].allowance+=2000;
    }
}

static int Game_upgradeProp(int propid, int flag)
{
    /*upgrade*/
    if (flag >= 0)
    {
        if (gProperties[propid].owner == cur_player && gProperties[propid].mortaged == 0 && gProperties[propid].upgrades < 5)
        {

            if (Game_isLegitUpgrade(cur_player, propid))
            {
                if (Game_players[cur_player].allowance>= gProperties[propid].upgradeCost)
                {
                    gProperties[propid].upgrades++;
                    Game_remove_money(cur_player, gProperties[propid].upgradeCost);
                }
                else
                {
                    message = "Not enough Taka.";
                }
            }
            else
            {
                message = "Not allowed to build";
            }
        }
        else if (gProperties[propid].owner == cur_player && gProperties[propid].mortaged == 0 && gProperties[propid].upgrades==5)
        {
            pop_up4(selectedProperty);
        }
        else
            message = "Can't build there";
    }
    /*downgrade*/
    else
    {
        if (gProperties[propid].owner == cur_player && gProperties[propid].mortaged == 0 && gProperties[propid].upgrades > 0 && Game_isLegitDowngrade(cur_player, propid))
        {
            gProperties[propid].upgrades--;
            Game_players[cur_player].allowance+= gProperties[propid].upgradeCost / 2;
        }
        else
        {
            message = "Can't destroy there";
        }
    }
    return 0;
}

static void pop_up3(int propid)
{
    const SDL_MessageBoxButtonData buttons[]=
    {
        {/*flags,buttonid,text*/ 0,0,"Sell"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Decline"},
    };

    const SDL_MessageBoxColorScheme colorScheme =
    {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata =
    {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "~Trade Property~", /* .title */
        "What's your choice?", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
    {
        SDL_Log("error displaying message box");
        return;
    }
    if (buttonid == -1)
    {
        SDL_Log("no selection");
    }
    if(buttonid==0)
    {
        gProperties[propid].owner=cur_player;
        Game_players[cur_player].allowance-=gProperties[propid].price;
        Game_players[(cur_player+1)%2].allowance+=gProperties[propid].price;
        return;
    }
    return;
}

static void trade_property(int propid)
{
    if(gProperties[propid].owner==(cur_player+1)%2 && gProperties[propid].upgrades==0)
    {
        pop_up3(propid);
    }
    printf("Not available for Trades.");
}

static void Game_goBankrupt()
{
    if (Game_players[cur_player].allowance< 0 || Game_players[cur_player].CGPA==2)
    {
        char * string = (char *) malloc(sizeof(char) * 40);
        int next_player = (cur_player + 1) % 2;
        sprintf(string, "%s is the Richie Rich of IUT", Game_players[next_player].name);
        message = string;
        gState = Game_STATE_END;
    }
}

void Game_receiveinput(SDL_Keycode key)
{
    if (key == SDLK_SPACE)
    {
        message = "";
        switch (gState)
        {
        case Game_STATE_BEGIN_MOVE:
            Game_cycle();
            break;
        case Game_STATE_BUY_PROPERTY:
            Game_buyProperty();
            if (lastRoll[0] != lastRoll[1])
                nextPlayer();
            break;
        case Game_STATE_PLAYER_IN_DEBT:
            Game_process_debts();
            break;
        default:
            break;
        }
    }
    if (key == SDLK_m)
    {
        if (selectedProperty >= 0 && selectedProperty < 40)
        {
            Game_mortageProp(selectedProperty);
        }
    }
    if (key == SDLK_x)
    {
        Game_goBankrupt();
    }
    if (key == SDLK_b)
    {
        Game_upgradeProp(selectedProperty, 1);
    }
    if (key == SDLK_d)
    {
        Game_upgradeProp(selectedProperty, -1);
    }
    if(key == SDLK_t)
    {
        trade_property(selectedProperty);
    }
}

void Game_getLastRoll(int* a, int* b)
{
    *a = lastRoll[0];
    *b = lastRoll[1];
}

char * Game_getText(int line)
{
    if (Game_STATE_END == gState)
    {
        if (line == 0)
            return message;
        else
            return NULL;
    }

    switch(line)
    {
    case 1:
        return "    M) Mortgage";
        break;
    case 2:
        return "    B) Build";
        break;
    case 3:
        return "    D) Destroy";
        break;
    case 4:
        return "    T) Trade";
    case 5:
        if (Game_STATE_BEGIN_MOVE == gState)
            return "SPACE) Roll and jump";
        if (Game_STATE_BUY_PROPERTY == gState)
            return "SPACE) Buy/Bid property";
        if (Game_players[cur_player].allowance< 0 || Game_players[cur_player].CGPA==2)
            return "    X) Defeat";
        return "";
        break;
    case 6:
        return message2;
        break;
    case 0:
        return message;
        break;
    }

    return NULL;
}

void Game_selectProperty(int propid)
{
    if (propid > 0 && propid < 40)
        selectedProperty = propid;
}

int Game_getPropLevel(int id)
{
    return gProperties[id].upgrades;
}

int Game_getPropMortageStatus(int id)
{
    return gProperties[id].mortaged;
}


