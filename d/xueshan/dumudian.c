// /d/xueshan/dumudian.c
// Summer 9/25/96
#include <ansi.h>
#include <room.h>

inherit ROOM;

nosave string current_id = "";
nosave int current_job = 0;

int introduce_robber(object current_player);
int reset_to_normal();

void create()
{
        set("short", "度母殿");
        set("long", @LONG
 这里是雪山寺的度母殿，正中五尊大座佛，均为高肉髻、外披袒
右袈裟、结跏趺坐、执手印须弭坐各异。殿内对称排列四根大柱，上
雕忍冬纹，下刻连珠纹。北壁上供有两尊金刚像：马头明王和阎魔护
法。
LONG );
        set("exits", ([
                                "west" : "/d/xueshan/zoulang1",
                ]));
        set("objects", ([
                "/d/xueshan/npc/zrlama": 1,
                "/d/xueshan/obj/tonggang": 1,
        ]));

        set("cost", 1);
    setup();
}

void reset()
{
        ::reset();
        set("no_clean_up", 1);
}

void init() {
    object challenger, me = this_player();

//    printf("blah blah blah\n");
    if( (query("family/family_name", me) == "雪山寺" || 
          query("family/family_name", me) == "血刀门" )
         && !(current_job)
          && query("combat_exp", me)>2000000){
//        printf("hehehe\n");
        call_out("introduce_robber", random(10)+10, me);
        current_job = 1;
    }
//    printf("current_job = %d\n",current_job);
}

int introduce_robber(object current_player)
{
    object robber;

//    printf("name = %s\n", current_player->query("id"));
        if (!current_player) return 0;
    if( objectp(present(query("id", current_player),this_object())) )
    {
//        printf("test1\n");
        robber = new("/d/xueshan/npc/robber");
//        printf("test2\n");
        robber->setup_robber(query("combat_exp", current_player));
        robber->move(this_object());
//        printf("test3\n");
//        message("vision", "一个"+robber->name(1)+"急急忙忙走了过来。\n", this_object(), robber);
        message_vision(HIC"一个$N急急忙忙走了过来。\n"NOR, robber);
        message_vision(HIR"$N说道：听说这个铜缸里存着喇嘛用的酥油，老子今天特地来看看，识相的赶快给我滚开！\n" NOR, robber, current_player);
        robber->start_checking(current_player);
//        printf("test4\n");
    }
    else
    {
        reset_to_normal();
    }
}

int reset_to_normal()
{

   current_job = 0;
   return 1;

}
