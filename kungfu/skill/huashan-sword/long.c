// This program is a part of NITAN MudLIB
// feilong.c 华山剑法「天外飞龙」

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string name() { return HIY "天外飞龙" NOR; }

int perform(object me, object target)
{
        int damage;
        string msg;
        object weapon;
        int ap, dp;

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail("「天外飞龙」只能在战斗中对对手使用。\n");

        if( !objectp(weapon=query_temp("weapon", me)) ||
            query("skill_type", weapon) != "sword" )
                return notify_fail("不拿剑怎么使用「天外飞龙」？\n");

        if ((int)me->query_skill("huashan-sword", 1) < 150 )
                return notify_fail("你华山剑法不够娴熟，使不出「天外飞龙」。\n");

        if ((int)me->query_skill("force") < 150 )
                return notify_fail("你内功火候不够，使不出「天外飞龙」。\n");

        if( query("neili", me)<400 )
                return notify_fail(HIC"你现在真气不够，无法将「天外飞龙」使完！\n"NOR);

        if (me->query_skill_mapped("sword") != "huashan-sword")
                return notify_fail("你没有激发华山剑法，无法使用「天外飞龙」。\n");

        msg = HIC "$N" HIC "心念电闪，一提内力，勒手仗剑，运劲于臂，呼"
              "的一声向$n" HIC "掷了出去，\n" NOR;

        ap = attack_power(me, "sword");
        dp = defense_power(target, "dodge");
        if (ap > dp / 2 * 3 && ap / 2 + random(ap) > dp)
        {
                me->start_busy(3);
                damage = target->query("max_qi");
                //damage = damage_power(me, "sword") * 3 / 2;
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 60,
                                           HIC "$n疾忙跃起，但剑如电闪，只觉一阵剧痛，剑刃"
                                           "拦腰而过，鲜血飞溅，皮肉卷起！\n" NOR);
                addn("neili", -400, me);
        } else
        {
                me->start_busy(3);
                msg += CYN "然而$n" CYN "轻轻一笑，侧身伸指，正弹"
                       "在$N" CYN "的剑脊上，剑斜飞落地。\n" NOR;
                addn("neili", -400, me);
        }

        weapon->move(environment(me));
        message_combatd(msg, me, target);
        return 1;
}