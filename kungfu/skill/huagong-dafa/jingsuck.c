// Code of ShenZhou
// jingsuck.c

#include <ansi.h>

inherit F_DBASE;
inherit F_SSERVER;

int exert(object me, object target)
{
        int sp, dp, jing, jing1, eff_jing;

        if( !target ) target = offensive_target(me);

        if( !objectp(target) || target == me)
                return notify_fail("你要吸取谁的精神？\n");

        if( query("no_fight", environment(me)) )
                return notify_fail("这里不能战斗，你不可以使用化功大法吸人的精神。\n");

        if( query_temp("sucked", me) )
                return notify_fail("你刚刚吸取过精神！\n");

        if( objectp(query_temp("weapon", me)) )
                return notify_fail("你的化功大法功力不够，必须空手才能施用化功大法吸人精神！\n");

        if( !me->is_fighting() || !target->is_fighting() || !living(target) )
                return notify_fail("你必须在战斗中才能吸取对方的精神！\n");

        if( (int)me->query_skill("huagong-dafa",1) < 45 )
                return notify_fail("你的化功大法功力不够，不能吸取对方的精神。\n");

        if( query("neili", me)<50 )
                return notify_fail("你的内力不够，不能使用化功大法。\n");

        if( query("jingli", me)<20 )
                return notify_fail("你的精力不够，不能使用化功大法。\n");

        if( query("jing", target)<query("max_jing", target)/5
          || query("jingli", target)<query("max_jingli", target)/10 )
                return notify_fail( target->name() +
                        "已经精神涣散，你已经无法从他体内吸取精力了！\n");

        message_vision(
                HIR "$N突然神秘的一笑，双掌一前一後对准$n的太阳穴拍了过来！\n\n" NOR,
                me, target );

        if( !target->is_killing(me) ) target->kill_ob(me);

        sp=me->query_skill("force")+me->query_skill("dodge")+query("kar", me)*50;
        dp=target->query_skill("force")+target->query_skill("dodge")+query("kar", target)*50;
        sp+=query("combat_exp", me)/1000;
        dp+=query("combat_exp", target)/1000;

        set_temp("sucked", 1, me);

        if( random(sp+dp) > dp )
        {
                tell_object(target, HIR "你猛觉额角两侧如针刺般剧痛，神志迷困，头脑中一片空白！\n" NOR);
                tell_object(me, HIG "你觉得" + target->name() + "的精力自手掌源源不绝地流了进来。\n" NOR);

                target->receive_damage("jing", (int)me->query_skill("huagong-dafa", 1)/5 );

                if( query("eff_jing", me)<query("max_jing", me) )
                        me->receive_curing("jing", (int)me->query_skill("huagong-dafa", 1)/10);
                else addn("jing", me->query_skill("huagong-dafa",1)/5, me);

                if( query("combat_exp", target) >= query("combat_exp", me)/2){
                        addn("potential", 1, me);
                        addn("combat_exp", 1, me);
                        if( query("potential", me)>me->query_potential_limit() )
                        set("potential", me->query_potential_limit(), me);
                }

                me->start_busy(random(3));
                target->start_busy(random(3));
                addn("neili", -me->query_skill("huagong-dafa",1)/6, me);
                addn("jingli", -5, me);

                call_out("del_sucked", 2, me);
        }
        else
        {
                addn("neili", -20, me);
                message_vision(HIY "可是$p看破了$P的企图，机灵地闪了开去。\n" NOR, me, target);
                me->start_busy(3);
                call_out("del_sucked", 4, me);
        }

        return 1;
}

void del_sucked(object me)
{
        if( query_temp("sucked", me) )
        delete_temp("sucked", me);
}
