// This program is a part of NITAN MudLIB

#include <ansi.h>
#include <combat.h>

string name() { return HIW "����ʽ" NOR; }

inherit F_SSERVER;

string final(object me, object target, int damage);

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp, skill;
        int damage;

        me->clean_up_enemy();
        if (! target) target = me->select_opponent();

        skill = me->query_skill("lonely-sword", 1);

        if (! me->is_fighting(target))
                return notify_fail(name() + "ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if (skill < 120)
                return notify_fail("����¾Ž��ȼ��������޷�ʩչ" + name() + "��\n");

        if( !objectp(weapon=query_temp("weapon", me) )
            || query("skill_type", weapon) != "sword" )
                return notify_fail("����ʹ�õ��������ԣ��޷�ʩչ" + name() + "��\n");

        if (me->query_skill_mapped("sword") != "lonely-sword")
                return notify_fail("��û�м������¾Ž����޷�ʩչ" + name() + "��\n");

        /*
        if (target->query_condition("no_exert"))
                return notify_fail("�Է������Ѿ��޷������������ŵ������ɡ�\n");
        */

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIW "$N" HIW "ʩ�����¾Ž�������ʽ��������" + weapon->name() +
              HIW "�й�ֱ����ңָ$n" HIW "������ҪѨ��\n" NOR;

        ap = attack_power(me, "sword") + me->query_skill("parry");
        dp = defense_power(target, "force") + target->query_skill("parry");

        if (ap / 2 + random(ap) > dp)
        {
                damage = damage_power(me, "sword");
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 55,
                                           (: final, me, target, damage :));
                me->start_busy(2);
        } else
        {
                msg += CYN "��$n" CYN "��֪$N" CYN "���е���"
                       "�����������������ߣ�����������\n" NOR;
                me->start_busy(3);
        }
        message_combatd(msg, me, target);

        return 1;
}

string final(object me, object target, int ap)
{
        if (target->query_condition("no_exert"))
                return "";
        target->apply_condition("no_exert", 10);
        call_out("poqi_end", 10 + random(20), me, target);
        return  HIR "$n" HIR "��$N" HIR "һ���������ţ��Ǹ�һ��"
                "׶�ĵĴ�ʹ��ȫ������ԴԴ��к��\n" NOR;
}

void poqi_end(object me, object target)
{
        if (target && target->query_condition("no_exert"))
        {
                if (living(target))
                {
                        message_combatd(HIC "$N" HIC "��������һ��������ɫ��"
                                        "��ת�죬�������ö��ˡ�\n" NOR, target);

                        tell_object(target, HIY "��е������ҵ���������ƽ��"
                                            "��������\n" NOR);
                }
                target->clear_condition("no_exert");
        }
        return;
}