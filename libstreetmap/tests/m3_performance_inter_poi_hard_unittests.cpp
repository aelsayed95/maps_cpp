//#include <algorithm>
//#include <unittest++/UnitTest++.h>
//#include "unit_test_util.h"
//#include "m3.h"
//#include "path_verify.h"
//
//SUITE(M3_Performance_Hard_Inter_POI) {
//	TEST(M3_Intersection_to_POI_Path_Performance_Hard) {
//		{
//		}
//		{
//			UNITTEST_TIME_CONSTRAINT(13513);
//			std::vector<unsigned> path;
//			path = find_path_to_point_of_interest(106544, "Subway");
//			path = find_path_to_point_of_interest(88340, "Subway");
//			path = find_path_to_point_of_interest(33719, "Tim Hortons");
//			path = find_path_to_point_of_interest(120633, "Tim Hortons");
//			path = find_path_to_point_of_interest(80551, "Subway");
//			path = find_path_to_point_of_interest(144529, "Tim Hortons");
//			path = find_path_to_point_of_interest(8357, "Tim Hortons");
//			path = find_path_to_point_of_interest(111791, "TD Canada Trust");
//			path = find_path_to_point_of_interest(31595, "Subway");
//			path = find_path_to_point_of_interest(38416, "Shoppers Drug Mart");
//			path = find_path_to_point_of_interest(140751, "TD Canada Trust");
//			path = find_path_to_point_of_interest(53624, "Shoppers Drug Mart");
//			path = find_path_to_point_of_interest(111670, "Starbucks Coffee");
//			path = find_path_to_point_of_interest(130104, "TD Canada Trust");
//			path = find_path_to_point_of_interest(2067, "Petro-Canada");
//			path = find_path_to_point_of_interest(41765, "Esso");
//			path = find_path_to_point_of_interest(35600, "Shoppers Drug Mart");
//			path = find_path_to_point_of_interest(21022, "Subway");
//			path = find_path_to_point_of_interest(135885, "Esso");
//			path = find_path_to_point_of_interest(8764, "Shoppers Drug Mart");
//			path = find_path_to_point_of_interest(38970, "Starbucks Coffee");
//			path = find_path_to_point_of_interest(152811, "Scotiabank");
//			path = find_path_to_point_of_interest(110970, "Esso");
//			path = find_path_to_point_of_interest(57205, "Esso");
//			path = find_path_to_point_of_interest(46215, "Petro-Canada");
//			path = find_path_to_point_of_interest(89236, "CIBC");
//			path = find_path_to_point_of_interest(88371, "Scotiabank");
//			path = find_path_to_point_of_interest(38594, "Esso");
//			path = find_path_to_point_of_interest(50258, "Pizza Pizza");
//			path = find_path_to_point_of_interest(129724, "Scotiabank");
//			path = find_path_to_point_of_interest(61064, "Shell");
//			path = find_path_to_point_of_interest(111982, "Shell");
//			path = find_path_to_point_of_interest(139802, "Scotiabank");
//			path = find_path_to_point_of_interest(41854, "Shell");
//			path = find_path_to_point_of_interest(142272, "Pizza Pizza");
//			path = find_path_to_point_of_interest(164215, "Second Cup");
//			path = find_path_to_point_of_interest(107157, "Second Cup");
//			path = find_path_to_point_of_interest(88188, "McDonald's");
//			path = find_path_to_point_of_interest(134745, "CIBC");
//			path = find_path_to_point_of_interest(62419, "Pizza Pizza");
//			path = find_path_to_point_of_interest(133137, "BMO Bank of Montreal");
//			path = find_path_to_point_of_interest(111245, "Coffee Time");
//			path = find_path_to_point_of_interest(50786, "Mr. Sub");
//			path = find_path_to_point_of_interest(80271, "RBC Financial Group");
//			path = find_path_to_point_of_interest(1070, "BMO Bank of Montreal");
//			path = find_path_to_point_of_interest(100010, "Pizza Nova");
//			path = find_path_to_point_of_interest(63932, "Mr. Sub");
//			path = find_path_to_point_of_interest(103491, "Mr. Sub");
//			path = find_path_to_point_of_interest(129840, "Pizza Nova");
//			path = find_path_to_point_of_interest(17511, "Swiss Chalet");
//			path = find_path_to_point_of_interest(139040, "RBC");
//			path = find_path_to_point_of_interest(50993, "KFC");
//			path = find_path_to_point_of_interest(97596, "BMO");
//			path = find_path_to_point_of_interest(149086, "Pizza Hut");
//			path = find_path_to_point_of_interest(121677, "Pizza Hut");
//			path = find_path_to_point_of_interest(99040, "KFC");
//			path = find_path_to_point_of_interest(18683, "Wendy's");
//			path = find_path_to_point_of_interest(7235, "HSBC");
//			path = find_path_to_point_of_interest(139428, "Toronto Sun");
//			path = find_path_to_point_of_interest(52870, "Country Style");
//			path = find_path_to_point_of_interest(47280, "Husky");
//			path = find_path_to_point_of_interest(38522, "National Bank");
//			path = find_path_to_point_of_interest(118857, "Bank of Montreal");
//			path = find_path_to_point_of_interest(131479, "Thai Express");
//			path = find_path_to_point_of_interest(100419, "Dental Office");
//			path = find_path_to_point_of_interest(152385, "Toronto Sun");
//			path = find_path_to_point_of_interest(165043, "Pharmasave");
//			path = find_path_to_point_of_interest(30135, "Domino's Pizza");
//			path = find_path_to_point_of_interest(106188, "National Bank");
//			path = find_path_to_point_of_interest(62523, "Wild Wing");
//			path = find_path_to_point_of_interest(6597, "Timothy's World Coffee");
//			path = find_path_to_point_of_interest(45353, "Pickle Barrel");
//			path = find_path_to_point_of_interest(151950, "Jimmy the Greek");
//			path = find_path_to_point_of_interest(28126, "California Sandwiches");
//			path = find_path_to_point_of_interest(129725, "Bell");
//			path = find_path_to_point_of_interest(100374, "Redbox");
//			path = find_path_to_point_of_interest(80493, "Fit for Life");
//			path = find_path_to_point_of_interest(78842, "Sunset Grill");
//			path = find_path_to_point_of_interest(46023, "Red Lobster");
//			path = find_path_to_point_of_interest(91989, "Baton Rouge");
//			path = find_path_to_point_of_interest(4393, "Union");
//			path = find_path_to_point_of_interest(95553, "The Burger's Priest");
//			path = find_path_to_point_of_interest(155818, "Spadina");
//			path = find_path_to_point_of_interest(45198, "Union");
//			path = find_path_to_point_of_interest(124511, "Treats");
//			path = find_path_to_point_of_interest(111624, "Baton Rouge");
//			path = find_path_to_point_of_interest(126059, "Coca-Cola Freestyle");
//			path = find_path_to_point_of_interest(31575, "Olco");
//			path = find_path_to_point_of_interest(139359, "Queen");
//			path = find_path_to_point_of_interest(149345, "Burrito Boyz");
//			path = find_path_to_point_of_interest(68089, "Medical Centre");
//			path = find_path_to_point_of_interest(99366, "Discount");
//			path = find_path_to_point_of_interest(22480, "Pita Pit");
//			path = find_path_to_point_of_interest(122999, "Dundas");
//			path = find_path_to_point_of_interest(67555, "ESSO");
//			path = find_path_to_point_of_interest(48564, "Bier Markt");
//			path = find_path_to_point_of_interest(75667, "Bayview");
//			path = find_path_to_point_of_interest(113859, "Bayview");
//			path = find_path_to_point_of_interest(103903, "St. Patrick");
//			path = find_path_to_point_of_interest(18886, "Pita & Grill");
//			path = find_path_to_point_of_interest(69159, "Ali Baba's");
//			path = find_path_to_point_of_interest(109173, "Warden");
//			path = find_path_to_point_of_interest(66174, "Desjardins");
//			path = find_path_to_point_of_interest(150921, "The Friendly Thai");
//			path = find_path_to_point_of_interest(105884, "Lawrence West");
//			path = find_path_to_point_of_interest(21253, "Casey's");
//			path = find_path_to_point_of_interest(40375, "IDA");
//			path = find_path_to_point_of_interest(106014, "Eglinton West");
//			path = find_path_to_point_of_interest(138403, "Foreign Exchange");
//			path = find_path_to_point_of_interest(7884, "The Spice Village");
//			path = find_path_to_point_of_interest(82325, "The Lawrence Park School");
//			path = find_path_to_point_of_interest(151297, "Pavo");
//			path = find_path_to_point_of_interest(31649, "Mandarin House");
//			path = find_path_to_point_of_interest(5496, "Mela Italian Vegetarian Magic");
//			path = find_path_to_point_of_interest(342, "Babur Indian Cuisine");
//			path = find_path_to_point_of_interest(30101, "Taste of Thailand cuisine");
//			path = find_path_to_point_of_interest(160100, "Mt. Pleasant");
//			path = find_path_to_point_of_interest(28001, "Dr. Julian Geller & FRIENDS Dental Care Specialists For Children And Young Adults");
//			path = find_path_to_point_of_interest(120545, "Harbour Square Post Office Souvenirs Gifts");
//			path = find_path_to_point_of_interest(22624, "The Big Mouth Luck Dumpling House");
//			path = find_path_to_point_of_interest(107918, "Double Double Pizza and Chicken");
//			path = find_path_to_point_of_interest(81322, "TARION");
//			path = find_path_to_point_of_interest(70124, "WonderPho");
//			path = find_path_to_point_of_interest(158594, "Hey Meatball");
//			path = find_path_to_point_of_interest(1974, "Dental Healthcare");
//			path = find_path_to_point_of_interest(61011, "Thompson");
//			path = find_path_to_point_of_interest(67360, "Paramount Fine Foods");
//			path = find_path_to_point_of_interest(83392, "aroma therapy toronto");
//			path = find_path_to_point_of_interest(12213, "Richard Charles Lee Resource Centre");
//			path = find_path_to_point_of_interest(152743, "Western Union");
//			path = find_path_to_point_of_interest(24092, "Petro-Canada Wholesale");
//			path = find_path_to_point_of_interest(164032, "Shouldice Hospital");
//			path = find_path_to_point_of_interest(38623, "Tiyam Grill");
//			path = find_path_to_point_of_interest(86792, "Grass Mountain Villa Seafood");
//			path = find_path_to_point_of_interest(13388, "Quigley's");
//			path = find_path_to_point_of_interest(79346, "Waterfront Medical Centre");
//			path = find_path_to_point_of_interest(91246, "DentalVille");
//			path = find_path_to_point_of_interest(96575, "Caf Chic");
//			path = find_path_to_point_of_interest(94026, "Airport Medical Centre");
//			path = find_path_to_point_of_interest(142321, "Loretto Abbey Catholic Secondary School");
//			path = find_path_to_point_of_interest(37845, "Kleinburg United Church");
//			path = find_path_to_point_of_interest(18323, "The Roti House");
//			path = find_path_to_point_of_interest(5343, "Millway Medical Centre");
//			path = find_path_to_point_of_interest(68326, "The Wallace Gastropub");
//			path = find_path_to_point_of_interest(96121, "Market Street Catch");
//			path = find_path_to_point_of_interest(56071, "Dentistry on Bay");
//			path = find_path_to_point_of_interest(158938, "Sushi 4-U");
//			path = find_path_to_point_of_interest(29735, "Victor");
//			path = find_path_to_point_of_interest(109103, "Best Portuguese Chicken");
//			path = find_path_to_point_of_interest(22217, "Linda Modern Thai Cuisine");
//			path = find_path_to_point_of_interest(2568, "Bikram Yoga Centre & Wellness Clinic");
//			path = find_path_to_point_of_interest(34687, "Sherman Health Science Research Centre");
//			path = find_path_to_point_of_interest(132554, "Faema Cafe");
//			path = find_path_to_point_of_interest(136419, "Tweedsmuir");
//		}
//	}
//}