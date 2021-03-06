// This program is a part of NITAN MudLIB
// hun.c 剑魂

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

string name() { return HIM "剑魂" NOR; }

int can_not_hubo() { return 1;}

void check_fight(object me, object target, object weapon);
private int remove_attack(object me, object weapon);

int perform(object me, object target)
{
        string msg;
        object *ob, weapon;
        int ap, dp, count, p;
        int i, damage;

        if( !query("reborn/times", me) )
                return notify_fail("你尚未转世重生，无法施展" + name() + "。\n");

        if( query("family/family_name", me) != "华山派"
         && member_array("华山派", query("reborn/fams", me)) == -1 )
                return notify_fail("你尚未转世重生，无法施展" + name() + "。\n");

/*
        if( query("family/family_name", me) != "华山派" )
                return notify_fail("你不是华山派弟子，无法施展" + name() + "。\n");
*/
        /*
        if( !target ) target = offensive_target(me);

        if( !target
        ||      !target->is_character()
        ||      !me->is_fighting(target) )
                return notify_fail(HUN "只能对战斗中的对手使用。\n");
        */

        if( !me->is_fighting() )
                return notify_fail(name() + "只能对战斗中的对手使用。\n");

        if( query_temp("dugu_jianhun", me) )
                return notify_fail("你已在使用" + name() + "了！\n");

        weapon=query_temp("weapon", me);
        if( !objectp(weapon) || query("skill_type", weapon) != "sword" )
                return notify_fail("你手中无剑，如何能够施展" + name() + "？\n");

        if( (int)me->query_skill("lonely-sword", 1) < 1000 )
                return notify_fail("你的独孤九剑不够娴熟，难以施展" + name() + "。\n");

        if( me->query_skill("force") < 1200 )
                return notify_fail("你的内功火候未到，无法配合杖法施展" + name() + "！\n");

        if( me->query_skill("sword", 1) < 1000 )
                return notify_fail("你剑法修为不足，还不会使用" + name() + "！\n");

        if( me->query_skill("sword-cognize", 1) < 500 )
                return notify_fail("你剑道修养火候未到，还不会使用" + name() + "！\n");

        if( me->query_skill("martial-cognize", 1) < 500 )
                return notify_fail("你武学修养火候未到，还不会使用" + name() + "！\n");

        if( query("neili", me) <= 1000 )
                return notify_fail("你的内力不够施展" + name() + "！\n");

        if( query("jingli", me) <= 500 )
                return notify_fail("你的精力不够施展" + name() + "！\n");

        count = me->query_skill("sword-cognize", 1) + me->query_skill("martial-cognize", 1);
        count /= 500;

        msg = HIW "$N" HIW "神态写意，温柔抚弄手中的" + weapon->name() + HIW "，只见" + weapon->name() +
              HIW "表面掠过一丝光影，微微颤动，整把剑竟灵动起来、有若生物。\n" NOR
              HIM"$N"HIM"将手中"+query("name", weapon)+HIM"缓缓刺出，剑招随意无章！\n"NOR
              HIM "这招看似缓慢，其实疾如闪电，看似随意无章，却是和" + weapon->name() + HIM "融为一体，无懈可击，蕴涵着返璞归真的意境。\n" NOR;

        message_combatd(msg, me);

        damage = damage_power(me, "sword");
        damage += query_temp("apply/damage", me);
        damage += query("jiali", me);
        damage *= 5;
        ap = attack_power(me, "sword") +
             me->query_skill("dodge");

        ob = me->query_enemy();
        addn_temp("apply/ap_power", 100, me);
        for (i = 0; i < sizeof(ob); i++)
        {
                dp = defense_power(ob[i], "parry") +
                     ob[i]->query_skill("dodge");

                if (ap * 2 / 3 + random(ap) > dp)
                {
                        ob[i]->receive_damage("qi", damage, me);
                        ob[i]->receive_wound("qi", damage / 2, me);
                        ob[i]->set_weak(5);
                        set("neili", 0, ob[i]);
                        set("jiali", 0, ob[i]);
                        ob[i]->apply_condition("no_exert", 10);
                        ob[i]->apply_condition("no_perform", 10);

                        msg = HIR "$n" HIR "想招架却感无从招架，想躲避也感到无处可躲，登时被$N" HIR "一剑划过，\n"
                              HIR "遭受重创，一阵锥心的刺痛，全身真气源源而泻！\n" NOR;

                        if( query_temp("apply/fatal_blow", me) && query("ability2/fatal_blow", me) )
                        {
                                if( random(100) < query("ability2/fatal_chance", me) )
                                {
                                        ob[i]->receive_damage("qi", query("qi", ob[i]) * query_temp("apply/fatal_blow", me)/100, me);
                                        ob[i]->receive_wound("qi", query("qi", ob[i]) * query_temp("apply/fatal_blow", me)/200, me);
                                        msg += HIR "紧接着，$N" HIR "追加了致命一击，给$n" HIR "造成更严重的创伤。\n" NOR;
                                }
                        }
                        p=query("qi", ob[i])*100/query("max_qi", ob[i]);
                        msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
                        message_vision(msg, me, ob[i]);
                } else
                {
                        ob[i]->start_busy(2);
                        msg = HIC "$n" HIC "情急之下，不及考虑，倒地一滚，侥幸避开$N" HIC "这一招的攻击，冷汗直淋！\n" NOR;
                        message_vision(msg, me, ob[i]);
                }
        } ;
        addn_temp("apply/ap_power", -100, me);
        me->start_busy(2);
        set_temp("dugu_jianhun", 1, me);
        addn("neili", -200, me);
        addn("jingli", -100, me);

        msg = HIY "$N" HIY "握紧" + weapon->name() + HIY "向天一指, 叫道 : 剑魂一出 ～ 血光漫天 ～ \n"
              HIY "顿时" + weapon->name() + HIY "剑身化出一道红色的剑形，在天空中恣意飞旋穿梭 ...\n" NOR;

        message_combatd(msg, me);
        call_out("check_fight", 1, me, weapon, count);

        return 1;
}

void check_fight(object me, object weapon, int count)
{
        object *ob;
        int damage, ap, dp, p;
        string msg, limb;
        string *limbs;
        int i;

        if( !objectp( me ) || !objectp(weapon) ) return;

        if( query_temp("dugu_jianhun", me) >= count
        ||      !me->is_fighting() )
        {
                call_out("remove_attack", 1, me, weapon);
                return;
        }

        addn_temp("dugu_jianhun", 1, me);
        message_combatd(HIR "\n剑魂以闪电般的高速在空中穿梭, 以狂乱无比的热情尽情狂舞 ...\n" NOR, me);

        ob = me->query_enemy();
        ap = me->query_skill("sword", 1) + me->query_skill("sword-cognize", 1) +
             me->query_skill("martial-cognize", 1);
        damage=ap+query("weapon_prop/damage", weapon);
        damage += damage_power(me, "sword");
        damage *= 5;

        for (i = 0; i < sizeof(ob); i++)
        {
                if (! objectp(ob[i])) continue;
                dp=query("parry", ob[i])+
                     ob[i]->query_skill("dodge", 1);

                if (dp / 3 + random(dp) < ap)
                {
                        limbs=query("limbs", ob[i]);
                        if (! arrayp(limbs))
                        {
                                limbs = ({ "身体" });
                                set("limbs", limbs, ob[i]);
                        }
                        limb = limbs[random(sizeof(limbs))];
                        ob[i]->start_busy(1);
                        ob[i]->receive_damage("qi", damage, me);
                        ob[i]->receive_wound("qi", damage / 2, me);
                        p=query("qi", ob[i])*100/query("max_qi", ob[i]);
                        msg = COMBAT_D->damage_msg(damage, "割伤");
                        msg = replace_string(msg, "$l", limb);
                        msg = replace_string(msg, "$w", weapon->name());
                        msg += "( $n"+COMBAT_D->eff_status_msg(p)+" )\n";
                        message_combatd(msg, me, ob[i]);
                }
        }

        call_out("check_fight", 1, me, weapon, count);
}

int remove_attack(object me, object weapon)
{
        if( !objectp( me ) ) return 1;
        if( !query_temp("dugu_jianhun", me))return 1;

        delete_temp("dugu_jianhun", me);
        message_combatd(HIW "一阵狂舞之后, 剑魂又自动飞入你的手中" + weapon->name() + HIW " ...\n" NOR, me);

        return 1;
}
