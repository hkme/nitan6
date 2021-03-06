// suck.c

#include <ansi.h>

inherit F_SSERVER;

int exert(object me, object target)
{
        int sp, dp;
        int my_max, tg_max;
        int amount;
        object weapon;

        if (! target || target == me) target = offensive_target(me);

        if( query("no_fight", environment(me)) )
                return notify_fail("在这里不能攻击他人。\n");

        if (! objectp(target) || ! me->is_fighting(target))
                return notify_fail("你只能吸取战斗中的对手的丹元！\n");

        if( query("race", target) != "人类" || 
            query("not_living", target) )
                return notify_fail("搞错了！只有活着的生物才能有丹元！\n");

        my_max=query("max_neili", me);
        tg_max=query("max_neili", target);

        if( query_temp("sucked", me) )
                return notify_fail("你刚刚吸取过丹元！\n");

        if (! me->is_fighting() || ! target->is_fighting())

        if ((int)me->query_skill("xixing-dafa", 1) < 100)
                return notify_fail("你的吸星大法尚未大成，还"
                                   "不能吸取对方的丹元收为己用！\n");

        if( query("neili", me)<100 )
                return notify_fail("你的内力不够，不能使用吸星大法。\n");

        if ((int)me->query_current_neili_limit() <= my_max)
                return notify_fail("你的内功水平有限，再吸取也是徒劳。\n");

        if( query("max_neili", target)<100 )
                return notify_fail( target->name() +
                        "丹元涣散，功力未聚，你无法从他体内吸取任何东西！\n");

        if( query("max_neili", target)<query("max_neili", me)/5 )
                return notify_fail( target->name() +
                        "的内功修为远不如你，你无法从他体内吸取丹元！\n");

        if( !objectp(weapon=query_temp("weapon", me)) )
                message_combatd(HIR "$N" HIR "探出右手，平平的拍在$n"
                                HIR "的胸前，似乎没有半点力道。\n\n" NOR,
                                me, target);
        else
                message_combatd(HIR "$N" HIR "把手中的" + weapon->name() +
                                HIR "一扬，慢慢的逼向$n" HIR + "，$p"
                                HIR "连忙架住。\n" NOR,
                                me, target);

        if (living(target) && !target->is_killing(me))
        {
                me->want_kill(target);
                target->kill_ob(me);
        }

        sp = me->query_skill("force");
        dp = target->query_skill("force");

        set_temp("sucked", 1, me);

        if ((sp + random(sp) > dp + random(dp) ) || ! living(target))
        {
                tell_object(target, HIR "你只觉全身乏力，全身功力如"
                            "融雪般消失得无影无踪！\n" NOR);
                tell_object(me, HIG "你觉得" + target->name() +
                            HIG "的丹元自手掌源源不绝地流了进来。\n" NOR);

                amount = 1 + (me->query_skill("xixing-dafa", 1) - 100) / 10;
                addn("max_neili", -amount, target);
                addn("max_neili", amount, me);
                addn("exception/xixing-count", amount*10, me);
                SKILL_D("xixing-dafa")->check_count(me);
                if( query("max_neili", target)<1 )
                        set("max_neili", 0, target);

                me->start_busy(5);
                target->start_busy(random(5));
                addn("neili", -10, me);

                call_out("del_sucked", 10, me);
        } else
        {        
                message_combatd(HIY "可是$p" HIY "看破了$P" HIY
                                "的企图，运用内力震开了$P" HIY
                                "，随即躲了开去。\n" NOR,
                                me, target);
                me->start_busy(5);
                call_out("del_sucked", 15, me);
        }

        return 1;
}

void del_sucked(object me)
{
        delete_temp("sucked", me);
}