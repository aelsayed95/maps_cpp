//#include <algorithm>
//#include <unittest++/UnitTest++.h>
//#include "unit_test_util.h"
//#include "m3.h"
//
///*
// * NOTE: this is the same test as in M3
// */
//
//SUITE(M4_Path_Cost) {
//	TEST(Path_Cost_0) {
//		std::vector<unsigned> path;
//		path = {172708, 172707, 172706, 172705, 203096, 203095, 186221, 186220, 186219, 186218, 207570, 207569, 207568, 207567, 207566, 207565, 207564, 155902, 155901, 155904, 155903, 173986, 173985, 134259, 134258, 186172, 54861, 54860, 156601, 156600, 54859, 54863, 54862, 186171, 186170, 118029, 118028, 118024, 118023, 196506, 118027, 118026, 118025, 206485, 206484, 206483, 183657, 183656, 183655, 183654, 183653, 183652, 183651, 183650, 183649, 183648, 183647, 183646, 183645, 183644, 183643, 183642, 183641, 183640, 198766, 198765, 183658, 151726, 151727, 183660, 183659, 207388, 207387, 207386, 207385, 207384, 207383, 207382, 207381, 207380, 207379, 207378, 207377, 207389, 207391, 207390, 22477, 22476, 22475, 22474, 22473, 22472, 116581, 116580, 22471, 22470, 39086, 39085, 39084, 39083, 39082, 203781, 203780, 203779, 203778, 203777, 203776, 203775, 203774, 203773, 203772, 203771, 203770, 203783, 203782, 203785, 203784, 191579, 191578, 191577, 191576, 191575, 27025, 27024, 27026, 27027, 39960, 39959, 39113, 39112, 39111, 39110, 39109, 39108, 39107, 189086, 189085, 189084, 189083};
//		double expected_travel_time = 7.487646927694801;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_1) {
//		std::vector<unsigned> path;
//		path = {189079, 189080, 196074, 196073, 196075, 196076, 196077, 196078, 196080, 193242, 196081, 196082, 193236, 193237, 193238, 193239, 193240, 193241, 65867, 65868, 65869, 65870, 65871, 65872, 65873, 59594, 59595, 59596, 59597, 59598, 59599};
//		double expected_travel_time = 1.505993767496339;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_2) {
//		std::vector<unsigned> path;
//		path = {59599, 59598, 59597, 59596, 59595, 59594, 65873, 65872, 65871, 65870, 65869, 65868, 65867, 193241, 193240, 193239, 193238, 193237, 193236, 196082, 196081, 193242, 196080, 196078, 196077, 196076, 196075, 196073, 196074, 189080, 189079, 189083, 189084, 189085, 189086};
//		double expected_travel_time = 2.036190656832275;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_3) {
//		std::vector<unsigned> path;
//		path = {39107, 39108, 39109, 39110, 39111, 39112, 39113, 39959, 39960, 27021, 27022, 27023};
//		double expected_travel_time = 0.7099792112052915;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_4) {
//		std::vector<unsigned> path;
//		path = {27026, 27027, 39960, 39959, 39113, 39112, 39111, 39110, 39109, 39108, 39107, 64033, 64032, 64036, 64035, 64034, 51199, 51198, 51197, 51196};
//		double expected_travel_time = 1.256055642916943;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_5) {
//		std::vector<unsigned> path;
//		path = {51195, 51194, 51193, 51213, 51212, 2548, 2549, 27127, 27128, 27129, 27130, 65872, 65873};
//		double expected_travel_time = 1.101817292006904;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_6) {
//		std::vector<unsigned> path;
//		path = {117445, 117446, 117447, 117448, 117453, 117452, 117451};
//		double expected_travel_time = 0.7236150505130189;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_7) {
//		std::vector<unsigned> path;
//		path = {117451, 117452, 117453, 117454, 117455, 51212, 51213, 51193, 51194, 51195, 51196, 51197, 51198, 51199, 64034, 64035, 64036, 64032, 64033, 39107, 39108, 39109, 39110, 39111, 39112, 39113, 39959, 39960, 27021, 27022, 27023, 27024, 27025, 191575, 191576, 191577, 191578, 191579, 203784, 203785, 203782, 203783, 203770, 203771, 203772, 203773, 203774, 203775, 203776, 203777, 203778, 203779, 203780, 203781};
//		double expected_travel_time = 2.946410927415323;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_8) {
//		std::vector<unsigned> path;
//		path = {194517, 194516, 194515, 194514, 194513, 39181, 39180};
//		double expected_travel_time = 0.3358707189821886;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_9) {
//		std::vector<unsigned> path;
//		path = {39180, 39181, 194513, 194514, 194515, 194516, 194517, 39082, 39083, 39084, 39085, 39086, 22470, 22471, 116580, 116581, 22472, 22473, 22474, 22475, 22476, 22477, 207390, 207391, 207389, 207377, 207378, 207379, 207380, 207381, 207382, 207383, 207384, 207385, 207386, 207387, 207388, 183659, 183660, 151727, 151726, 183658, 198765, 198766, 183640, 183641, 183642, 183643, 183644, 183645, 183646, 183647, 183648, 183649, 183650, 183651, 183652, 183653, 183654, 183655};
//		double expected_travel_time = 3.075970203958764;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_10) {
//		std::vector<unsigned> path;
//		path = {183655, 183654, 183653, 183652, 183651, 183650, 183649, 183648, 183647, 183646, 183645, 183644, 183643, 183642, 183641, 183640, 198766, 198765, 183658, 151726, 151727, 183660, 183659, 207388, 207387, 207386, 207385, 207384, 207383, 207382, 207381, 207380, 207379, 207378, 207377, 207389, 207391, 207390, 22477, 22476, 22475, 22474, 22473, 22472, 116581, 116580, 22471, 22470, 39086, 39085, 39084, 39083, 39082, 203781, 203780, 203779, 203778, 203777, 203776, 203775, 203774, 203773, 203772, 203771, 203770, 203783, 203782, 203785, 203784, 191579, 191578, 191577, 191576, 191575, 27025, 27024, 27026, 27027, 39960, 39959, 39113, 39112, 39111, 39110, 39109, 39108, 39107, 64033, 64032, 64036, 64035, 64034, 51199, 51198, 51197, 51196, 51195, 51194, 51193, 51213, 51212, 117455, 117454, 117453, 117452, 117451, 117450, 117449, 22429, 22428, 22427, 22426, 22425, 113506, 113505, 148783, 183921, 183922, 179438, 179439, 179440, 148782, 185838, 65874};
//		double expected_travel_time = 6.748016432290687;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_11) {
//		std::vector<unsigned> path;
//		path = {65876, 65875, 200062, 200061, 200060, 76719, 76718, 76717, 76716, 76715, 76714, 76713, 76712, 39667, 39668, 39669, 39670, 39671, 39672, 39673, 39674, 39675, 117445, 117446, 117447, 117448};
//		double expected_travel_time = 1.728788268930983;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_12) {
//		std::vector<unsigned> path;
//		path = {117454, 117455, 51212, 51213, 51193, 51194, 51195, 51196, 51197, 51198, 51199, 64034, 64035, 64036, 64032, 64033, 39107, 39108, 39109, 39110, 39111, 39112, 39113};
//		double expected_travel_time = 1.208236889530139;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_13) {
//		std::vector<unsigned> path;
//		path = {39959, 39960, 27021, 27022, 27023, 27024, 27025, 191575, 191576, 191577, 191578, 191579, 203784, 203785, 203782, 203783, 203770, 203771, 203772, 203773, 203774, 203775, 203776, 203777, 203778, 203779, 203780, 203781, 1589, 1590, 1591, 1592, 1593, 1594, 213106, 213107, 213108};
//		double expected_travel_time = 2.269040392829857;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_14) {
//		std::vector<unsigned> path;
//		path = {213108, 213107, 213106, 1594, 1593, 1592, 1591, 1590, 1589, 203781, 203780, 203779, 203778, 203777, 203776, 203775, 203774, 203773, 203772, 203771, 203770, 203783, 203782, 203785, 203784, 191579, 191578, 191577, 191576, 191575, 27025, 27024, 27026, 27027, 39960, 39959, 39113, 39112, 39111, 39110, 39109, 39108, 39107, 64033, 64032, 64036, 64035, 64034, 51199, 51198, 51197, 51196, 51195, 51194, 51193, 51213, 51212, 117455, 117454, 117453, 117452, 117451, 59593, 59592, 59591, 59590};
//		double expected_travel_time = 4.204406682805605;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_15) {
//		std::vector<unsigned> path;
//		path = {59589, 59588, 59649, 59648, 59647, 59646, 59645, 76716, 76715, 76714, 76713, 76712, 27119, 27120, 27121, 16063, 16064, 16065, 16066, 73722, 73723, 134475, 189803, 189804, 189805, 189806, 189807, 189808, 189809, 189077, 189076};
//		double expected_travel_time = 2.117830151035699;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_16) {
//		std::vector<unsigned> path;
//		path = {189558, 189557, 189556, 189555, 189554, 189553, 189552, 189551, 189550, 189549, 189548, 189547, 189546, 189545, 189544, 189561, 189560, 189559, 199882, 199881, 199880, 199879, 199878, 199877, 199876, 199875, 199874, 199873, 199872, 199871, 2119, 2118, 2117, 199884, 199883, 36119, 36118, 207012, 207011, 207010, 207009, 207008, 207007, 207006, 207005, 207004, 207003};
//		double expected_travel_time = 3.096317878365721;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_17) {
//		std::vector<unsigned> path;
//		path = {10815, 10816, 155459, 155460, 155461, 155462, 155463, 186680, 186681, 5585, 5586, 16581, 16582, 16071, 16072, 16073, 378, 374, 375, 376, 377, 16074, 16075, 16076, 16077, 192258, 192259, 192260, 192261, 192262, 192263, 192264, 388, 389, 390, 391, 392, 393, 394, 395, 396, 45730, 45731, 45724, 45725, 45726, 45727, 45728, 45729, 10831, 10832, 45762, 45763, 45764};
//		double expected_travel_time = 2.881518691241647;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_18) {
//		std::vector<unsigned> path;
//		path = {45764, 45763, 45762, 10832, 10831, 45729, 45728, 45727, 45726, 45725, 45724, 16963, 16964, 16965, 16966, 16967, 16968, 6190, 6191, 6192, 134255, 134256, 134251, 134252, 4777, 4778, 4779, 4780, 4781, 4782, 4783, 189078, 134478, 134479, 135190, 135189, 135188, 135187, 135186, 135185, 135184, 159695, 159696};
//		double expected_travel_time = 2.860565598466663;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_19) {
//		std::vector<unsigned> path;
//		path = {159696, 159695, 135184, 135185, 135186, 135187, 135188, 135189, 135190, 134476, 134477, 134480, 134481, 134482, 134483, 134484, 134485, 134486, 134487, 134488, 134489, 134490, 134491, 134492, 51200, 51201, 51196, 51197, 51198, 51199, 64034, 64035, 64036, 64032, 64033, 39107, 39108, 39109, 39110, 39111, 39112, 39113, 39959, 39960, 27021, 27022, 27023, 27024, 27025, 191575, 191576, 191577, 191578, 191579, 203784, 203785};
//		double expected_travel_time = 3.451888541501;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_20) {
//		std::vector<unsigned> path;
//		path = {203785, 203784, 191579, 191578, 191577, 191576, 191575, 27025, 27024, 27026, 27027, 39960, 39959, 39113, 39112, 39111, 39110, 39109, 39108, 39107, 64033, 64032, 64036, 64035, 64034, 51199, 51198, 51197, 51196, 51201, 51200, 134492, 134491, 134490};
//		double expected_travel_time = 2.321548458466794;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_21) {
//		std::vector<unsigned> path;
//		path = {193241, 193240, 193239, 193238, 193237, 193236, 196082, 196081, 193242, 196080, 196078, 196077, 196076, 196075, 196073, 196074, 189080, 189079, 22411, 22410, 22409, 22408, 22400, 22399, 22398, 39958, 39957, 39956, 39955, 192104, 192103, 5038, 5037, 192957, 192956, 45770, 192954, 192955, 141747, 141746, 176184, 176183, 176182, 186646, 186645, 176186, 176185, 183361, 183360, 183359, 183358, 183357, 183356, 183355, 183354, 186637, 186636, 186635, 186634, 186633, 186628, 186630, 186629, 186632, 186631, 186640, 186639, 186638, 27029, 27028, 194512, 194511, 154800, 154799, 154798, 154797, 154796, 154795};
//		double expected_travel_time = 3.977218552026467;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_22) {
//		std::vector<unsigned> path;
//		path = {198583, 198582, 39360, 39359, 39358, 39357, 5591, 5590, 5589, 5587, 5588, 366, 367, 368, 369, 370, 371, 372, 373, 16583, 16584, 16585, 16586, 16587, 16581, 16582, 16071, 16072, 16073, 378, 374, 375, 376, 377, 16074, 16075, 16076, 16077, 192258, 192259, 192260, 192261, 192262, 192263, 192264, 388, 389, 390, 391, 392, 393, 394, 395, 396, 45730, 45731, 16963, 16964, 16965, 16966, 16967, 16968, 6190, 6191, 6192, 134255, 134256, 134251, 134252, 4777, 4778, 4779, 4780, 4781, 4782, 4783, 189078, 134478, 134479, 73724, 73725, 73726, 73727, 16060, 16061, 16062, 76712, 76713, 76714, 76715, 76716, 76717, 76718, 76719, 200060, 200061, 141582, 141581, 141580, 141585, 213509, 213515, 213514, 213513, 213511, 213523, 213524, 135195, 135194, 135193, 135192, 135191};
//		double expected_travel_time = 5.663442339465505;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//	TEST(Path_Cost_23) {
//		std::vector<unsigned> path;
//		path = {135191, 135192, 135193, 135194, 135195, 213524, 213523, 213511, 213513, 213514, 213515, 213509, 141585, 141580, 141581, 141582, 200061, 200060};
//		double expected_travel_time = 1.361288037771741;
//		double actual_travel_time = compute_path_travel_time(path);
//		CHECK(ece297test::relative_error(expected_travel_time, actual_travel_time) < 0.001);
//	}
//
//}
//
