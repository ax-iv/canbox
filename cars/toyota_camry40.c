static void toyota_camry_40_ms_wheel_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.wheel = 0;
		return;
	}

	int16_t angle = (int16_t)(((uint16_t)(msg[0] & 0x0F)) << 8 | msg[1]);
	angle = (angle < 2048) ? angle : (angle - 4096);

	carstate.wheel = scale(angle, -380, 380, -100, 100);
}

static void toyota_camry_40_ms_speed_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.speed = 0;
		return;
	}
	carstate.speed = ((((uint16_t)msg[5]) << 8 | msg[6]) + 50) / 100;
}

static void toyota_camry_40_ms_ign_brake_doors_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.acc = STATE_UNDEF;
		carstate.ign = STATE_UNDEF;
		carstate.park_break = STATE_UNDEF;
		carstate.fl_door = STATE_UNDEF;
		carstate.fr_door = STATE_UNDEF;
		carstate.rl_door = STATE_UNDEF;
		carstate.rr_door = STATE_UNDEF;
		carstate.tailgate = STATE_UNDEF;
		carstate.ds_belt = STATE_UNDEF;
		return;
	}
	carstate.acc        = (msg[4] & 0x10) ? 1:0;
	carstate.ign 		= (msg[4] & 0x20) ? 1:0;
	carstate.park_break = (msg[7] & 0x10) ? 0:1;
	carstate.fl_door 	= (msg[5] & 0x20) ? 1:0;
	carstate.fr_door 	= (msg[5] & 0x10) ? 1:0;
	carstate.rl_door 	= (msg[5] & 0x08) ? 1:0;
	carstate.rr_door 	= (msg[5] & 0x04) ? 1:0;
	carstate.tailgate 	= (msg[5] & 0x01) ? 1:0;
	carstate.ds_belt    = (msg[7] & 0x40) ? 1:0;
}

static void toyota_camry_40_ms_light_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.illum = STATE_UNDEF;
		carstate.near_lights = STATE_UNDEF;
		carstate.park_lights = STATE_UNDEF;
		return;
	}
	/*
	0x622 четвертый байт 0x40 бит — светит дальний
0x622 четвертый байт 0x20 бит — светит ближний
0x622 четвертый байт 0x10 бит — светят габариты
0x622 четвертый байт 0x08 бит — светят передние противотуманки
0x622 четвертый байт 0x04 бит — светят задние противотуманки*/
	carstate.illum 			= (msg[3] & 0x10) ? 100:0;
	carstate.near_lights 	= (msg[3] & 0x20) ? 1:0;
	carstate.park_lights    = (msg[3] & 0x10) ? 1:0;
}

static void toyota_camry_40_ms_drive_mode_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
	 	carstate.selector = STATE_UNDEF;
		return;
	}

	if ((msg[4] & 0xF0) == 0x80)
		carstate.selector = e_selector_p;
	else if ((msg[4] & 0xF0) == 0x40)
		carstate.selector = e_selector_r;
	else if ((msg[4] & 0xF0) == 0x20)
		carstate.selector = e_selector_n;
	else if ((msg[4] & 0xF0) == 0x00 && msg[5] == 0x40)
		carstate.selector = e_selector_d;
	else if ((msg[4] & 0xF0) == 0x00 && (msg[5] == 0x00 || msg[5] == 0x01))
		carstate.selector = e_selector_s;
	else
		carstate.selector = STATE_UNDEF;
}

static void toyota_camry_40_ms_odometer(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.odometer = 0;
		return;
	}
	carstate.odometer = ((uint32_t)msg[5] << 16) | ((uint32_t)msg[6] << 8) | ((uint32_t)msg[7] << 0);
}

static void toyota_camry_40_ms_tacho_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.taho = 0;
		return;
	}
	carstate.taho = ((uint32_t)msg[0] << 8) | ((uint32_t)msg[1] << 0);
	carstate.taho = ((carstate.taho * 3) + 2) / 4;
}

static void toyota_camry_40_ms_temp_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.temp = 0;
		return;
	}

	carstate.temp = (int32_t)((int8_t)msg[3]-0x30);
}

static void toyota_camry_40_ms_park_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.radar.state = e_radar_off;
		return;
	}

	if (msg[0] == 0x1B) {

		carstate.radar.state = e_radar_on_rear;
/*
ID 396 DLC — 7 Data: 1B 56 70 56 A0 0F E0
первые два байта 1B (18 — кнопка датчиков отключена, 19 — кнопка включена, 1B — поступает сигнал с датчиков и на приборной панели появляется пиктограма машинки с полосками)
вторые два байта 56 — передние датчики (5 — левый 6 — правый, значения могут быть 6 — три полоски, 5 — две полоски, 1 — одна мигающая полоска)
третьи два байта 70 — задний средний датчик (датчика два, но полоска одна на экране для двух датчиков, меняется только один бит, первый, значения могут быть 70 — четыре плоски, 60 — три полоски, 50 — две полоски, 10 — одна мигающая полоска)
четвертые два байта 56 — заднии крайнии датчики ( 5 — левый 6 — правый, значения могут быть 6 — три полоски, 5 — две полоски, 1 — одна мигающая полоска)
*/
		carstate.radar.fl = 99 - scale((msg[1] & 0xF0)>>4, 0x0, 0x6, 0, 99);
		carstate.radar.flm = 99;
		carstate.radar.frm = 99;
		carstate.radar.fr = 99 - scale((msg[1] & 0xF), 0x0, 0x6, 0, 99);
		
		carstate.radar.rl = 99 - scale((msg[3] & 0xF0)>>4, 0x0, 0x6, 0, 99);
		carstate.radar.rlm = 99 - scale(msg[2], 0x0, 0x70, 0, 99);
		carstate.radar.rrm = 99 - scale(msg[2], 0x0, 0x70, 0, 99);
		carstate.radar.rr = 99 - scale(msg[3] & 0xF, 0x0, 0x6, 0, 99);

	}
	else if (msg[0] == 0x18)
		carstate.radar.state = e_radar_off;
	else
		carstate.radar.state = e_radar_off;
}

static void toyota_camry_40_ms_air_set_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
/*0x381 второй байт 0x01 бит — AC
0x381 второй байт 0x04 бит — Рециркуляция воздуха
0x381 второй байт 0x08 бит — Автоматическая рециркуляция воздуха
0x381 второй байт 0x10 бит — Обогрев заднего стекла и зеркал
0x381 второй байт 0x20 бит — Максимальный обдув лобового стекла
0x381 второй байт 0x80 бит — Auto режим
0x381 третий байт 0x20 бит — Dual режим
0x381 третий байт 0x40 бит — Обогрев дворников
0x381 третий байт 0x07 биты — скорость обдува
0x381 шестой байт 0x01 бит — Нажата кнопка Mode
0x381 четвертый байт 0xF0 биты — Направление обдува (0x10 — в лицо, 
0x20 — в ноги и лицо, 0x30 — в ноги, 0x40 — в ноги и секло
*/
	if (is_timeout(desc)) {

		car_air_state.fanspeed = STATE_UNDEF;
		car_air_state.ac = STATE_UNDEF;
		car_air_state.rear = STATE_UNDEF;		
		return;
	}
	car_air_state.ac = (msg[1] & 0x01);
	car_air_state.recycling = (msg[1] & 0x04)>>2;
	car_air_state.dual = (msg[2] & 0x20)>>5;
	car_air_state.wind = 0;
	car_air_state.middle = 0;
	car_air_state.floor = 0;
	switch((msg[3] & 0xF0)>>4){
		case 1: car_air_state.middle=1; break;
		case 2: car_air_state.middle=1; car_air_state.floor = 1; break;
		case 3: car_air_state.floor = 1; break;
		case 4: car_air_state.wind = 1; car_air_state.floor = 1; break;
		default: break;
	}
	car_air_state.fanspeed = msg[2] & 0x07;
	car_air_state.aqs = (msg[1]&0x08)>0 ? 1 : 0;
	car_air_state.powerfull = (msg[1] & 0x20) > 0 ? 1 : 0;
	
}

static void toyota_camry_40_ms_air_temp_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
/*
0x382 первый байт — Установленная температура слева по формуле — 16+X/2.
0x382 второй байт — Установленная температура справа по формуле — 16+X/2.*/
	if (is_timeout(desc)) {

		car_air_state.fanspeed = STATE_UNDEF;
		car_air_state.ac = STATE_UNDEF;
		car_air_state.rear = STATE_UNDEF;
		return;
	}
	car_air_state.l_temp = 16+msg[0]/2;
	car_air_state.r_temp = 16+msg[1]/2;
}

static void  toyota_camry_40_ms_fuel_handler(const uint8_t * msg, struct msg_desc_t * desc){
	if (is_timeout(desc)) {
		carstate.fuel_lvl = STATE_UNDEF;
		carstate.low_fuel_lvl = STATE_UNDEF;
		return;
	}
	if(((msg[0] & 0xFF) ==  0x03) && msg[3]){
		carstate.low_fuel_lvl = (msg[3]/2) <= 5 ? 1: 0;
		carstate.fuel_lvl = (msg[3]/2); //scale ??
	}
	
}

struct msg_desc_t toyota_camry_40_ms[] =
{
	{ 0x025,   80, 0, 0, toyota_camry_40_ms_wheel_handler },
	{ 0x0b4,   100, 0, 0, toyota_camry_40_ms_speed_handler },
	{ 0x620,   200, 0, 0, toyota_camry_40_ms_ign_brake_doors_handler },
	{ 0x622,   1000, 0, 0, toyota_camry_40_ms_light_handler },
	{ 0x3b4,   1000, 0, 0, toyota_camry_40_ms_drive_mode_handler},
	{ 0x611,   1000, 0, 0, toyota_camry_40_ms_odometer},
	{ 0x2c4,   100, 0, 0, toyota_camry_40_ms_tacho_handler},
	{ 0x3b0,   2000, 0, 0, toyota_camry_40_ms_temp_handler},
	{ 0x396,   100, 0, 0, toyota_camry_40_ms_park_handler},
	{ 0x381,   1000, 0, 0, toyota_camry_40_ms_air_set_handler},
	{ 0x382,   1000, 0, 0, toyota_camry_40_ms_air_temp_handler},
	{ 0x7C8,   10000, 0, 0,toyota_camry_40_ms_fuel_handler},
	
};

