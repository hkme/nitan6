#include <ansi.h>
#include <combat.h>

string name() { return HIW "辟邪劍影" NOR; }

inherit F_SSERVER;

int perform(object me, object target)
{
        string msg;
        string name;
        object weapon;
        int i;
        int skill;
        int count;

        if( !query("reborn/times", me) )
                return notify_fail("你还没有转世重生，还不能体会到" + name() + "的奥秘。\n");

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! me->is_fighting(target))
                return notify_fail(name() + "只能对战斗中的对手使用。\n");

        if( me->is_busy() )
                return notify_fail("你正忙着呢。\n");

        weapon=query_temp("weapon", me);

        if (! weapon)
                return notify_fail("你使用的武器不对，难以施展" + name() + "。\n");

        if( weapon && query("skill_type", weapon) != "sword" && 
            query("skill_type", weapon) != "pin" )
                return notify_fail("你使用的武器不对，难以施展" + name() + "。\n");

        skill = me->query_skill("pixie-jian", 1);

        if (skill < 1000)
                return notify_fail("你的辟邪剑法不够娴熟，难以施展" + name() + "。\n");

        if (me->query_skill("dodge", 1) < 1000)
                return notify_fail("你基本轻功不够娴熟，难以施展" + name() + "。\n");

        if( query("neili", me)<1000 )
                return notify_fail("你现在的真气不足，难以施展" + name() + "。\n");

        if (weapon && me->query_skill_mapped("sword") != "pixie-jian")
                return notify_fail("你没有激发辟邪剑法，难以施展" + name() + "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        msg = HIR "$N" HIR "身形一转，手中" + weapon->name() + HIR "寒光闪烁，猛然间，化作"
              "万千剑影射向$n" HIR "，正是江湖中失传已久的惊世奇学「辟邪劍影」！\n" NOR;

        message_combatd(sort_msg(msg), me, target);

        count = attack_power(me, "sword");
        addn_temp("apply/attack", count*2, me);
        addn_temp("apply/damage", count, me);
        addn_temp("apply/unarmed_dage", count, me);

        addn_temp("pixie-jian/hit_msg", 1, me);

        set_temp("pixie-jian/pfm_jian", 1, me);

        for (i = 0; i < 12; i++)
        {
                if (! me->is_fighting(target))
                        break;

                if (random(2) == 0 && ! target->is_busy())
                        target->start_busy(1);

                COMBAT_D->do_attack(me, target, weapon, 0);
        }
        addn("neili", -800, me);
        if (random(5) != 1)
                me->start_busy(1 + random(3));
        addn_temp("apply/attack", -(count*2), me);
        addn_temp("apply/damage", -count, me);
        addn_temp("apply/unarmed_dage", -count, me);
        delete_temp("pixie-jian/hit_msg", me);
        delete_temp("pixie-jian/pfm_jian", me);
        return 1;
}