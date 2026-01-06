#include "configor/json.hpp"

const configor::json drink = {
		{"Mojito", {
			{"name", "Mojito"},
			{"name_zh", "莫吉托"},
			{"ingredients", {"白朗姆 50 ml", "青柠 20 ml", "糖 2 勺", "薄荷叶 8-10", "苏打水"}},
			{"price", 68},
			{"abv", 14},
			{"taste", "薄荷凉、青柠酸、气泡清爽"},
			{"mood", "夏日街头、周青春、无酒精版友好"},
			{"description", "古巴传奇调酒，清凉薄荷与酸爽青柠的完美碰撞，夏日解暑圣品"}
		}},
		{"Long Island Iced Tea", {
			{"name", "Long Island Iced Tea"},
			{"name_zh", "长岛冰茶"},
			{"ingredients", {"伏特加 15 ml", "金酒 15 ml", "朗姆 15 ml", "龙舌兰 15 ml", "橙酒 15 ml", "酸甜 25 ml", "可乐补满"}},
			{"price", 88},
			{"abv", 28},
			{"taste", "可乐、柠檬、烈"},
			{"mood", "买醉、失恋、学生党"},
			{"description", "四种基酒伪装成冰茶的经典之作，外表温和实则烈性十足"}
		}},
		{"Margarita", {
			{"name", "Margarita"},
			{"name_zh", "玛格丽特"},
			{"ingredients", {"龙舌兰 50 ml", "橙味利口酒 20 ml", "青柠汁 20 ml", "盐边"}},
			{"price", 80},
			{"abv", 26},
			{"taste", "酸、咸、龙舌兰植物香"},
			{"mood", "海边度假、失恋疗愈、派对开场"},
			{"description", "龙舌兰的灵魂之作，盐边与酸橙的戏剧性组合，墨西哥国饮"}
		}},
		{"Blue Hawaii", {
			{"name", "Blue Hawaii"},
			{"name_zh", "蓝色夏威夷"},
			{"ingredients", {"白朗姆 45 ml", "蓝橙酒 15 ml", "菠萝汁 60 ml", "椰奶 30 ml"}},
			{"price", 78},
			{"abv", 16},
			{"taste", "椰子、菠萝、海洋色"},
			{"mood", "泳池浮床、度假、拍照打卡"},
			{"description", "视觉与味觉的双重热带风暴，蓝色的海洋风情装在杯里"}
		}},
		{"Negroni", {
			{"name", "Negroni"},
			{"name_zh", "尼格罗尼"},
			{"ingredients", {"金酒 30 ml", "金巴利 30 ml", "红威末 30 ml", "橙皮 1 片"}},
			{"price", 85},
			{"abv", 28},
			{"taste", "苦甜平衡、草本、柑橘尾韵"},
			{"mood", "餐前开胃、老友重逢、初尝苦酒"},
			{"description", "意式苦甜哲学的结晶，金巴利的草本苦与金酒的杜松子香完美交融"}
		}},
		{"Piña Colada", {
			{"name", "Piña Colada"},
			{"name_zh", "椰林飘香"},
			{"ingredients", {"白朗姆 60 ml", "椰奶 60 ml", "菠萝汁 90 ml", "碎冰"}},
			{"price", 82},
			{"abv", 15},
			{"taste", "椰香、菠萝、奶昔"},
			{"mood", "泳池派对、热带假期、无酒精版"},
			{"description", "波多黎各的流动假期，椰子菠萝的奶昔式调酒，度假模式启动器"}
		}},
		{"Bomb Shot", {
			{"name", "Bomb Shot"},
			{"name_zh", "炸弹酒"},
			{"ingredients", {"功能饮料 150 ml", "野格 30 ml 或 伏特加 30 ml", "shot杯"}},
			{"price", 55},
			{"abv", 18},
			{"taste", "甜、气泡、烈酒冲击"},
			{"mood", "夜店、游戏、快速上头"},
			{"description", "派对文化符号，能量饮料与烈酒的狂欢式碰撞，一饮而尽的快感"}
		}},
		{"Clover Club", {
			{"name", "Clover Club"},
			{"name_zh", "三叶草俱乐部"},
			{"ingredients", {"金酒 45 ml", "覆盆子糖浆 20 ml", "柠檬汁 20 ml", "蛋清"}},
			{"price", 82},
			{"abv", 22},
			{"taste", "莓果酸甜、绵密泡沫、少女粉"},
			{"mood", "闺蜜之夜、拍照打卡、酸甜控"},
			{"description", "费城绅士俱乐部的粉色浪漫，蛋清带来云朵般的绵密口感"}
		}},
		{"Cosmopolitan", {
			{"name", "Cosmopolitan"},
			{"name_zh", "大都会"},
			{"ingredients", {"伏特加 40 ml", "君度 15 ml", "蔓越莓汁 30 ml", "青柠汁 15 ml"}},
			{"price", 78},
			{"abv", 22},
			{"taste", "酸甜、蔓越莓、粉红"},
			{"mood", "欲望都市、闺蜜、粉红滤镜"},
			{"description", "《欲望都市》中的都市女性标志，蔓越莓的粉红酸甜诱惑"}
		}},
		{"Strawberry Daiquiri", {
			{"name", "Strawberry Daiquiri"},
			{"name_zh", "草莓代基里"},
			{"ingredients", {"白朗姆 50 ml", "草莓 60 g", "糖浆 15 ml", "青柠汁 20 ml"}},
			{"price", 76},
			{"abv", 20},
			{"taste", "草莓、酸甜、冰沙"},
			{"mood", "少女、冰沙、拍照"},
			{"description", "古巴代基里的草莓变奏，新鲜草莓的冰沙式甜蜜革命"}
		}},
		{"White Lady", {
			{"name", "White Lady"},
			{"name_zh", "白色佳人"},
			{"ingredients", {"金酒 45 ml", "君度 25 ml", "柠檬汁 20 ml", "可选蛋清"}},
			{"price", 80},
			{"abv", 26},
			{"taste", "顺滑、柑橘、柔和"},
			{"mood", "优雅女、复古、轻熟"},
			{"description", "金酒的优雅三重奏，君度的柑橘香与柠檬的酸爽完美平衡"}
		}},
		{"Zombie", {
			{"name", "Zombie"},
			{"name_zh", "僵尸"},
			{"ingredients", {"混合朗姆 75 ml", "青柠汁 25 ml", "菠萝汁 45 ml", "百香果糖浆 15 ml", "苦精 1 dash"}},
			{"price", 98},
			{"abv", 30},
			{"taste", "果香、朗姆、烈"},
			{"mood", "Tiki派对、限量两杯、断片警告"},
			{"description", "Tiki文化的终极烈酒怪兽，三种朗姆打造的复活节岛传奇"}
		}},
		{"Martini", {
			{"name", "Martini"},
			{"name_zh", "马天尼"},
			{"ingredients", {"伦敦干金 60 ml", "干威末 10 ml", "柠檬皮或橄榄"}},
			{"price", 95},
			{"abv", 34},
			{"taste", "干冽、杜松子、丝滑"},
			{"mood", "商务谈判、詹姆斯·邦德、优雅正装"},
			{"description", "鸡尾酒之王，007的标志性选择，干与烈的终极平衡艺术"}
		}},
		{"Rum", {
			{"name", "Rum"},
			{"name_zh", "朗姆酒"},
			{"ingredients", {"纯饮 - 白朗姆或陈酿朗姆 45 ml", "或加冰直饮"}},
			{"price", 40},
			{"abv", 40},
			{"taste", "甘蔗甜香、橡木、焦糖"},
			{"mood", "加勒比海、航海、自由"},
			{"description", "加勒比海盗的液体黄金，甘蔗蒸馏的航海灵魂，调酒界的万能基酒"}
		}},
		{"Beer", {
			{"name", "Beer"},
			{"name_zh", "啤酒"},
			{"ingredients", {"拉格或艾尔啤酒 330 ml", "可加柠檬片"}},
			{"price", 35},
			{"abv", 5},
			{"taste", "麦香、清爽、微苦"},
			{"mood", "看球赛、烧烤、解渴"},
			{"description", "人类最古老的酒精饮料，从美索不达米亚流淌至今的欢乐泉水"}
		}},
		{"Vodka", {
			{"name", "Vodka"},
			{"name_zh", "伏特加"},
			{"ingredients", {"纯饮 - 伏特加 45 ml", "或作为基酒使用"}},
			{"price", 45},
			{"abv", 40},
			{"taste", "纯净、中性、轻微谷物香"},
			{"mood", "战斗民族、调酒基底、直接"},
			{"description", "斯拉夫文化的纯净灵魂，蒸馏技术的极致追求，调酒界的隐形冠军"}
		}}
	};

const configor::json medicine = {
    {"Bujiale", {
        {"name", "Estradiol Valerate Tablets"},
        {"name_zh", "补佳乐"},
        {"ingredients", {"戊酸雌二醇 1mg/片"}},
        {"indications", "与孕激素联合使用建立人工月经周期，补充雌激素缺乏（血管舒缩性疾病、生殖泌尿道营养性疾病、睡眠障碍）"},
        {"price", 40},
        {"description", "雌激素补充剂，用于建立人工月经周期及缓解雌激素缺乏症状"}
    }},
    {"Soprolone", {
        {"name", "Cyproterone Acetate"},
        {"name_zh", "色谱龙（醋酸环丙孕酮）"},
        {"ingredients", {"醋酸环丙孕酮"}},
        {"indications", "合成甾体抗雄激素和黄体制剂，用于雄激素依赖性疾病"},
        {"price", 85},
        {"description", "抗雄激素类药物"}
    }},
    {"Spironolactone", {
        {"name", "Spironolactone"},
        {"name_zh", "内螺酯（螺内酯）"},
        {"ingredients", {"螺内酯"}},
        {"indications", "利尿剂，用于水肿性疾病、高血压等（基于药理学分类信息）"},
        {"price", 15},
        {"description", "保钾利尿剂，用于水肿和高血压治疗"}
    }},
    {"Lithium_Carbonate", {
        {"name", "Lithium Carbonate"},
        {"name_zh", "碳酸锂"},
        {"ingredients", {"碳酸锂 0.25g/片"}},
        {"indications", "躁狂症、双相情感障碍"},
        {"price", 45},
        {"description", "心境稳定剂，用于治疗躁狂症及双相情感障碍"}
    }},
    {"Quetiapine", {
        {"name", "Quetiapine Fumarate"},
        {"name_zh", "喹硫平"},
        {"ingredients", {"富马酸喹硫平"}},
        {"indications", "精神分裂症、双相情感障碍"},
        {"price", 100},
        {"description", "抗精神病药物，用于治疗精神分裂症和双相情感障碍"}
    }},
    {"Venlafaxine", {
        {"name", "Venlafaxine"},
        {"name_zh", "文拉法辛"},
        {"ingredients", {"文拉法辛"}},
        {"indications", "SNRI类抗抑郁药，用于治疗抑郁症等精神障碍"},
        {"price", 75},
        {"description", "抗抑郁药"}
    }},
    {"Dextromethorphan", {
        {"name", "Dextromethorphan"},
        {"name_zh", "右美沙芬"},
        {"ingredients", {"氢溴酸右美沙芬"}},
        {"indications", "中枢性镇咳药，用于干咳"},
        {"price", 20},
        {"description", "中枢性镇咳药，用于干咳治疗"}
    }},
    {"Olanzapine", {
        {"name", "Olanzapine"},
        {"name_zh", "奥氮平"},
        {"ingredients", {"奥氮平 5mg/片"}},
        {"indications", "精神分裂症、双相情感障碍"},
        {"price", 200},
        {"description", "抗精神病药物，用于治疗精神分裂症和双相情感障碍"}
    }},
    {"Clonazepam", {
        {"name", "Clonazepam"},
        {"name_zh", "氯硝西泮"},
        {"ingredients", {"氯硝西泮"}},
        {"indications", "失眠、焦虑、躁狂症辅助治疗、双相情感障碍"},
        {"price", 30},
        {"description", "苯二氮䓬类药物，用于抗焦虑和镇静催眠"}
    }}
};

const configor::json evidence = {
    {"evidence_1", {
        {"name", "证物-解剖记录"},
        {"description", "证物-解剖记录\n死亡时间为昨日晚间，\n死因为亚硝酸钠中毒导致高铁血红蛋白症"}
    }},
    {"evidence_2", {
        {"name", "证物-亚硝酸钠"},
        {"description", "证物-亚硝酸钠\n一瓶胶囊，内含致死量亚硝酸钠。"}
    }}
};