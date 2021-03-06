// zhixin.c

#include <ansi.h>

int exert(object me, object target)
{
        int level = me->query_skill("linji-zhuang", 1);

        if (level < 60) return notify_fail("你的临济十二庄修为还不够。\n");

        if( query("max_neili", me)<5*level )
                return notify_fail("你的内力还不够强。\n");

        if( query("neili", me)<4*level )
                return notify_fail("你的真气不够。\n");

        if( query("eff_qi", me)<query("max_qi", me)/2 )
                return notify_fail("你已经受伤过重，只怕一运真气便有生命危险！\n");

        if( query_temp("linji/zhixin", me) )
                return notify_fail("你已经运用之心二庄凝聚心神了。\n");

        set_temp("linji/zhixin", 1, me);
        write( HIY "你微一凝神，运动之心两庄，去浊气出体外，收清气入心中，只觉灵台清明，领悟力似乎有所增加。\n" NOR);
        message("vision",
                HIY + "只见" + me->name() + "微一凝神，吞吐几口长气，一对眼眸灵动晶亮，神采弈弈。\n" NOR,
                environment(me), me);

        addn("neili", -4*level, me);
        level += me->query_skill("literate", 1);
        level = 1+random(level)/16;
        addn_temp("apply/int", level, me);
        set("jiali", 0, me);

        me->start_call_out( (: call_other, __FILE__, "recover", me, level :), 1800);

        return 1;
}

void recover(object me, int level)
{
        addn_temp("apply/int", -level, me);
        delete_temp("linji/zhixin", me);

        tell_object(me, HIG"你体内已积过多浊气，似乎领悟力又归于寻常。\n"NOR);
}