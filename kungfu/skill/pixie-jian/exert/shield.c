// shield.c 护体神功
//

#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int skill;

        if (target != me)
                return notify_fail("你只能用辟邪剑法来提升自己的防御力。\n");

        if( query("neili", me)<100 )
                return notify_fail("你的内力不够。\n");

        if ((int)me->query_skill("pixie-jian", 1) < 50)
                return notify_fail("你的辟邪剑法修为不够。\n");

        if( query_temp("shield", me) )
                return notify_fail("你已经在运功中了。\n");

        skill = me->query_skill("force");
        addn("neili", -100, me);
        me->receive_damage("qi", 0);

        message_combatd(HIW "$N" HIW "身子忽前忽后，忽左忽右，一"
                        "层层气浪跌宕翻涌，护住全身！\n" NOR, me);

        addn_temp("apply/armor", skill*3/2, me);
        set_temp("shield", 1, me);

        me->start_call_out((: call_other, __FILE__, "remove_effect", me, skill * 3 / 2 :), skill);

        if (me->is_fighting()) me->start_busy(2);

        return 1;
}

void remove_effect(object me, int amount)
{
        addn_temp("apply/armor", -amount, me);
        delete_temp("shield", me);
        tell_object(me, "你的辟邪剑法运行完毕，将内力收回丹田。\n");
}
