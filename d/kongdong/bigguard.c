//  秦天彪：龙门镖局总镖头  /d/city/lanzhou/npc/bigguard.c
//  等级：
//  【丙等】大门派的二级师傅、中等门派的一级师傅，江湖上知名的豪客
//  ★★★★☆
//  〖一级〗四级修为（１５０１－１９００）五级武功（  ８０－１２０）
//          经验（  １００万－  ２００万）声望（  １００００－  ２００００）
//          内力（  ２２００－  ３０００）体能、生命（  ７５０－    ９５０）
//  By Lin
//  Changed Lala, 1998-03-25

#include <ansi.h>
string ask_skills();
inherit NPC;
int CanTeach();



void create()
{
    set_name( "秦天彪", 
        ({ "qin tianbiao", "qin", "qintianbiao", "tianbiao", "master" }) );
    set( "title", HIW"龙门镖局总镖头"NOR );
    set( "nickname", CYN"关西第一刀"NOR );
    set( "gender", "男性" );                //性别：男
    set( "age", 72 );                       //年龄：52
    set( "str", 30 );                       //臂力：30
    set( "cor", 30 );                       //胆识：30
    set( "dex", 30 );                       //身法：30
    set( "per", 22 );                       //容貌：22
    set( "int", 35 );
    set( "rank_info/respect", "老英雄" );   //敬称：老英雄
    set( "positive_score", 18000 );         //正声望：18000
    set( "negative_score",  2000 );         //负声望：2000
    set( "long", @LONG
    秦天彪少年时就生得异常魁梧，早年曾在少林寺当了很多年的俗家弟子，后
来又得高人相授，刀法和铁砂掌功夫都是相当了得。现在是龙门镖局的大当家。
LONG
    );

    set_wugong( "blade",        120,    120,    0 );    //横刀六式：    一级
    set_wugong( "ditang-dao",   120,    120,    0 );    //地趟刀：      二级
    set_wugong( "wuhu-dao",     100,    100,    0 );    //五虎断门刀：  三级
    set_wugong( "kuaidao",      105,    105,    1 );    //快刀十三式：  四级
    set_wugong( "fenglei-dao",  150,    150,    0 );    //风雷刀：      五级

    set_wugong( "unarmed",      120,    120,    0 );    //基本拳法：    一级
    set_wugong( "luohan-quan",  100,    100,    0 );    //罗汉拳：      一级
    set_wugong( "taizu-quan",   120,    120,    0 );    //太祖长拳：    二级
    set_wugong( "weituo-zhang",  80,     80,    0 );    //韦陀掌：      二级
    set_wugong( "tiesha-zhang", 150,    150,    1 );    //铁砂掌：      三级

    set_wugong( "qinggong",     120,    120,    0 );    //基本轻功：    一级
    set_wugong( "feiyan-zoubi", 120,    120,    0 );    //飞檐走壁之术：二级
    set_wugong( "feiyan-gong",  100,    100,    1 );    //飞燕功：      三级      
    set_wugong( "caiyun-gong",   90,     90,    1 );    //彩云功：      四级
    set_wugong( "xueying-jian", 100,    100,    1 );

    set_wugong( "banruo-xinfa",120,    120,    1 );    //少林心法

    set_xiuwei_by_type( "blade",    3000 ); //刀法修为：    五级
    set_xiuwei_by_type( "unarmed",  1800 ); //拳脚修为：    三级
    set_xiuwei_by_type( "dodge",    1800 ); //轻功修为：    三级
    set_xiuwei_by_type( "zonggang",  1000 ); //总纲修为：    一级

    set( "max_force",   2700 );             //内力：    2700
    set( "force",       2700 );
    set( "force_factor", 135 );             //加力：    135 (5%)

    set_hp( "kee",      875 );              //生命、体能：  875
    set_hp( "sen",      875 );

    set( "combat_exp",  1000000 );          //战斗经验：100万
    set( "social_exp",   170000 );          //江湖阅历：17万
    set( "talk_msg", ({
        "“这位英雄来我镖局有何贵干？”",
    }) );
    set("inquiry",([
        "武功"   :    (: ask_skills :),
        "skills" :    (: ask_skills :),
    ]) );
    setup();

    carry_object( CLOTH_DIR + "cloth5" )->wear();       //紫绸长衫
    add_money( "silver", 5 );               //有银子五两
       
}

void init()
{
    object ppl;

    ::init();

    if ( !userp( ppl = this_player() ) 
        || !can_act() ) 
        return;

    remove_call_out( "Greeting" );
    call_out( "Greeting", 1, ppl );
}

void Greeting( object ppl )
{       
     
    if( !ppl 
        || environment( ppl ) != environment() ) 
        return;
    if( ppl->query_temp( "marks_lin/kill_wang" ) )       //如果此人杀过王太医
    {
        message_vision( "秦天彪向$N愤怒地说道：“想不到有人竟敢欺负到我的头上"
            "？！”\n", ppl );
        message_vision( "秦天彪对$N大喝道：“今天为王老丈报仇！”\n", ppl );
        kill_ob( ppl );
        ppl->fight_ob( this_object() );
        return;
     }
    command( "say “这位英雄来我镖局有何贵干？”" );
    command( "look "+ ppl->query( "id" ) );
}

int refuse_killing( object ppl )            //被杀时的反应
{
    object npc = this_object();

    if( !living( npc ) 
        || query_temp( "weapon" ) )
        return 0;
    message_vision( "$N看了$n一眼，冷笑道：“居然有你这么不知天高地厚的狂妄之"
        "徒，哼哼，待老夫今天好好收拾你！！\n", npc, ppl );
    new( BLADE_DIR "blade2" )->move( npc ); //拿一把单刀
    command( "wield blade" );               
    command( "enable fenglei-dao" );         //使用风雷刀
    command( "disable kuaidao" );            //不使用快刀
    return 0;
}

int killed_enemy( object ppl )              //杀死敌人的反应
{
    if ( ppl->query( "marks_lin/kill_wang" ) )//玩家删除杀死王太医的记录
        ppl->delete( "marks_lin/kill_wang" );
    ppl->delete_temp( "marks_lin/" );
    return 1;
}

int valid_teach( object ppl, string skill )
{
	command( "say “以后要多替为师押镖，闲着多练练般若心法。”" );
    if ( ppl->query( "marks/lin/兰州秘密B/送官" )!=1)      //如果没有学功夫的标记
    {
        command( "say “武林中人，要多为朝廷办事？”" );
        return 0;
    }

    if ( !query_jibie( skill ) )
        return 0;

    if ( skill == "blade"              //高级的武功不给学
        || skill == "unarmed" 
        || skill == "qinggong" )
    {
        command( "say “这些功夫嘛，太基本了，没工夫教你啊！”" );
        return 0;
    }
    
    return 1; 
}

int zhongcheng( object ppl, string skill )  //不检查忠诚度
{
    return 1;
}

string ask_skills()
{ 
    command( "sigh" );
   return "为师离开少林后，跟过很多名师，象地趟刀法、快刀十三式、\n
           五虎断门刀、风雷刀以及彩云功等都曾学过。\n
           可惜象相思刀法、胡家刀法、杀人刀法都不曾学会。";
   
}