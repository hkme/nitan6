#include <ansi.h>
#include <combat.h>

#define ZHEN "��" HIW "�𾪰���" NOR "��"

inherit F_SSERVER;

int perform(object me, object target)
{
        int damage;
        string msg;
        int ap, dp;

        if( userp(me) && !query("can_perform/xianglong-zhang/zhen", me) )
                return notify_fail("����ʹ�õ��⹦��û�����ֹ��ܡ�\n");

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(ZHEN "ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if( query_temp("weapon", me) || query_temp("secondary_weapon", me) )
                return notify_fail(ZHEN "ֻ�ܿ���ʹ�á�\n");

        if ((int)me->query_skill("xianglong-zhang", 1) < 150)
                return notify_fail("�㽵��ʮ���ƻ�򲻹�������ʩչ" ZHEN "��\n");

        if (me->query_skill_mapped("strike") != "xianglong-zhang")
                return notify_fail("��û�м�������ʮ���ƣ�����ʩչ" ZHEN "��\n");

        if (me->query_skill_prepared("strike") != "xianglong-zhang")
                return notify_fail("��û��׼������ʮ���ƣ�����ʩչ" ZHEN "��\n");

        if ((int)me->query_skill("force") < 300)
                return notify_fail("����ڹ���Ϊ����������ʩչ" ZHEN "��\n");

        if( query("max_neili", me)<3000 )
                return notify_fail("���������Ϊ����������ʩչ" ZHEN "��\n");

        if( query("neili", me)<500 )
                return notify_fail("�����ڵ��������㣬����ʩչ" ZHEN "��\n");

        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = WHT "$N" WHT "ʩ������ʮ����֮��" HIW "�𾪰���" NOR +
              WHT "����ȫ�������Ķ���˫������ɽ������ѹ��$n" WHT "��\n"NOR;  

        ap=me->query_skill("strike")+query("str", me)*10;
        dp=target->query_skill("dodge")+query("dex", target)*10;

        if (ap / 2 + random(ap) > dp)
        { 
                damage = ap + random(ap / 2);
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 80,
                                           HIR "$n" HIR "ֻ��һ���ӿ����������"
                                           "����ܣ�$N" HIR "˫������ǰ�أ���Ѫ��"
                                           "���������\n" NOR);
                addn("neili", -400, me);
                me->start_busy(3);
        } else
        {
                msg += CYN "$n" CYN "�ۼ�$N" CYN "������ӿ��˿����"
                       "��С���æ������һ�ԡ�\n" NOR;
                addn("neili", -200, me);
                me->start_busy(4);
        }
        message_combatd(msg, me, target);

        return 1;
}