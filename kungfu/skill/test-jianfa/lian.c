#include <ansi.h>
#include <combat.h>

#define PFM "「" HIW "正在测试自创武功系统" NOR "」"

#define PMSKILLS_D "/adm/daemons/pmskillsd"

inherit F_SSERVER;

string *weapon_sk = ({
        "sword", "blade", "staff", "whip", "club"
});

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int count;
        int i, attack_time;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(PFM "只能对战斗中的对手使用。\n");

        if( member_array(query("pmskills/types", me),weapon_sk) != -1 )
        {
                if( !objectp(weapon=query_temp("weapon", me) )
                    || query("skill_type", weapon) != query("pmskills/types", me) )
                        return notify_fail("你所使用的武器不对，难以施展" PFM "。\n");
        }
        else
        {
                if( query_temp("weapon", me) || query_temp("secondary_weapon", me) )
                        return notify_fail(PFM "只能空手使用。\n");                
        }
        if( me->query_skill(query("pmskills/skid", me),1)<260 )
                return notify_fail("你"+CHINESE_D->chinese(query("pmskills/skid", me))+
                                   "不够娴熟，难以施展" PFM "。\n");

        if( member_array(query("pmskills/types", me),weapon_sk) == -1 )
        {
                if( me->query_skill_prepared(query("pmskills/types", me)) != query("pmskills/skid", me) )
                        return notify_fail("你没有准备"+CHINESE_D->chinese(query("pmskills/skid", me))+
                                           "，难以施展" PFM "。\n");
        }
        if( query("neili", me)<300 )
                return notify_fail("你现在的真气不够，难以施展" PFM "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        msg = HIW "sd" + "\n" + NOR;

        ap=me->query_skill(query("pmskills/types", me));
        dp = target->query_skill("dodge");
        attack_time = 5;

        if (ap / 2 + random(ap * 2) > dp)
        {
                msg += HIM "sdf" + "\n" NOR;
                count = ap / 8;
                count+=PMSKILLS_D->get_point(query("pmskills/point", me));
               
                addn_temp("apply/attack", count, me);
        } else
        {
                msg += NOR + CYN "dfds" + "\n" NOR;
                count = 0;
        }

        message_sort(msg, me, target);

        if( PMSKILLS_D->get_point(query("pmskills/point", me))>10)attack_time+=2;

        if (attack_time > 13)
                attack_time = 13;

        addn("neili", -attack_time*20, me);

        set_temp("pmskill/lian", 1, me);
        for (i = 0; i < attack_time; i++)
        {
                if (! me->is_fighting(target))
                        break;

                if (random(2))target->start_busy(1);

                COMBAT_D->do_attack(me,target,(member_array(query("pmskills/types", me),weapon_sk) == -1?0:weapon),0);
        }
        addn_temp("apply/attack", -count, me);
        delete_temp("pmskill/lian", me);
        me->start_busy(1 + random(attack_time));

        return 1;
}