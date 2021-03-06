// This program is a part of NT MudLIB
// suxing.c 素心诀

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string name() { return "素心决"; }

void remove_effect(object me, int skill);

int perform(object me, object target)
{
        string msg;
        object weapon;
        int damage,ap,dp,married;

        if( query("gender", me) != "女性" )
                return notify_fail("玉女素心只能是女的用！\n");

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail("素心诀只能对战斗中的对手使用。\n");

        if( me->query_skill_mapped("force") != "yunv-xinfa" )
                return notify_fail("你所用的并非玉女心法，无法施展素心诀！\n");

        if( me->query_skill("yunv-xinfa", 1) < 200 )
                return notify_fail("你的玉女心法火候未到，无法施展素心诀！\n");

        if( me->query_skill("yunv-jian", 1) < 200 )
                return notify_fail("你的玉女素心剑法火候未到，无法施展素心诀！\n");

        if( !(weapon=query_temp("weapon", me)) || query("skill_type", weapon) != "sword" )
                return notify_fail("你使用的兵器不对。\n");

        if( !stringp(me->query_skill_mapped("sword")) ||
            me->query_skill_mapped("sword") != "yunv-jian")
                return notify_fail("你必须激发玉女素心剑作为剑法才能够使用素心诀。\n");

        if( query("neili", me)<1000 )
                return notify_fail("你的内力不够使用玉女素心！\n");

        if( query("jingli", me)<500 )
                return notify_fail("你的精力不够使用玉女素心！\n");

        message_combatd(HIW "$N突然面色庄重，双目清澄，手中" +
                       query("name", weapon)+HIW+"蒙上了一层白霜，寒气逼人！\n\n"NOR,me);
        message_combatd(HIW "$N腾身而起，姿态无比婀娜飘逸，挥剑向$n直刺过去，快愈流星！\n\n"NOR,me,target);

        ap = attack_power(me, "sword") + me->query_skill("force");
        dp = defense_power(target, "parry") + me->query_skill("force");

        if( mapp(query("family", target)) && query("family/family_name", target) == "全真教" )
                ap += ap/3;

        if( query("can_learn/yunv-xinfa/wall", me) )
                ap *= 2;

        if (ap/2 + random(ap) > dp || !living(target))
        {
                damage = damage_power(me, "sword");
                if( query("can_learn/yunv-xinfa/wall", me) ) damage *= 2;
                //判断结婚次数和发生性行为的次数
                if( !mapp(query("couple", me)))married=1;
                else married=query("couple/married", me);
                if( mapp(query("sex", me)))married+=query("sex/times", me);
                damage -= (damage/5) * (married - 1);
                if (damage < 10) damage = 10;
/*
                target->receive_damage("qi", damage/2 + random(damage/2), me);
                target->receive_wound("qi", damage, me);
*/
                msg=HIR"只听$n一声惨呼，已然给$N的"+NOR+query("name", weapon)+HIR+
                      "重重的刺中，浑身上下凝结上一层白色冰霜，冷的瑟瑟发抖！\n"NOR;
                msg = COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 200, msg);
                if (!target->is_busy())
                        target->start_busy(1+random(2));
                me->start_busy(2);
        } else
        {
                msg = HIY "$n见状大惊失色，双足点地，抽身急退，堪堪避过$N的这记素心诀，已是大汗淋漓。\n"NOR;
                me->start_busy(2+random(4));
        }
        message_combatd(msg, me, target);
        return 1;
}
