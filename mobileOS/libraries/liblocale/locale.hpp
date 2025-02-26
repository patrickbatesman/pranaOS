/**
 * @file locale.hpp
 * @author Krisna Pranav
 * @brief Country Codes
 * @version 1.0
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2023 2021-2023, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <array>
#include <string>

namespace Utils::country {

    /**
     * @brief Country ID
     * 
     */
    enum class ID {
        AFGHANISTAN,
        ALAND_ISLANDS,
        ALBANIA,
        ALGERIA,
        AMERICAN_SAMOA,
        ANDORRA,
        ANGOLA,
        ANGUILLA,
        ANTARCTICA,
        ANTIGUA_AND_BARBUDA,
        ARGENTINA,
        ARMENIA,
        ARUBA,
        AUSTRALIA,
        AUSTRIA,
        AZERBAIJAN,
        BAHAMAS,
        BAHRAIN,
        BANGLADESH,
        BARBADOS,
        BELARUS,
        BELGIUM,
        BELIZE,
        BENIN,
        BERMUDA,
        BHUTAN,
        BOLIVIA,
        BOSNIA_AND_HERZEGOVINA,
        BOTSWANA,
        BOUVET_ISLAND,
        BRAZIL,
        BRITISH_VIRGIN_ISLANDS,
        BRITISH_INDIAN_OCEAN_TERRITORY,
        BRUNEI_DARUSSALAM,
        BULGARIA,
        BURKINA_FASO,
        BURUNDI,
        CAMBODIA,
        CAMEROON,
        CANADA,
        CAPE_VERDE,
        CAYMAN_ISLANDS,
        CENTRAL_AFRICAN_REPUBLIC,
        CHAD,
        CHILE,
        CHINA,
        HONG_KONG_SAR_CHINA,
        MACAO_SAR_CHINA,
        CHRISTMAS_ISLAND,
        COCOS_KEELING_ISLANDS,
        COLOMBIA,
        COMOROS,
        CONGO_BRAZZAVILLE,
        CONGO_KINSHASA,
        COOK_ISLANDS,
        COSTA_RICA,
        COTE_DIVOIRE,
        CROATIA,
        CUBA,
        CYPRUS,
        CZECH_REPUBLIC,
        DENMARK,
        DJIBOUTI,
        DOMINICA,
        DOMINICAN_REPUBLIC,
        ECUADOR,
        EGYPT,
        EL_SALVADOR,
        EQUATORIAL_GUINEA,
        ERITREA,
        ESTONIA,
        ETHIOPIA,
        FALKLAND_ISLANDS_MALVINAS,
        FAROE_ISLANDS,
        FIJI,
        FINLAND,
        FRANCE,
        FRENCH_GUIANA,
        FRENCH_POLYNESIA,
        FRENCH_SOUTHERN_TERRITORIES,
        GABON,
        GAMBIA,
        GEORGIA,
        GERMANY,
        GHANA,
        GIBRALTAR,
        GREECE,
        GREENLAND,
        GRENADA,
        GUADELOUPE,
        GUAM,
        GUATEMALA,
        GUERNSEY,
        GUINEA,
        GUINEABISSAU,
        GUYANA,
        HAITI,
        HEARD_AND_MCDONALD_ISLANDS,
        HOLY_SEE_VATICAN_CITY_STATE,
        HONDURAS,
        HUNGARY,
        ICELAND,
        INDIA,
        INDONESIA,
        IRAN_ISLAMIC_REPUBLIC_OF,
        IRAQ,
        IRELAND,
        ISLE_OF_MAN,
        ISRAEL,
        ITALY,
        JAMAICA,
        JAPAN,
        JERSEY,
        JORDAN,
        KAZAKHSTAN,
        KENYA,
        KIRIBATI,
        KOREA_NORTH,
        KOREA_SOUTH,
        KUWAIT,
        KYRGYZSTAN,
        LAO_PDR,
        LATVIA,
        LEBANON,
        LESOTHO,
        LIBERIA,
        LIBYA,
        LIECHTENSTEIN,
        LITHUANIA,
        LUXEMBOURG,
        MACEDONIA_REPUBLIC_OF,
        MADAGASCAR,
        MALAWI,
        MALAYSIA,
        MALDIVES,
        MALI,
        MALTA,
        MARSHALL_ISLANDS,
        MARTINIQUE,
        MAURITANIA,
        MAURITIUS,
        MAYOTTE,
        MEXICO,
        MICRONESIA_FEDERATED_STATES_OF,
        MOLDOVA,
        MONACO,
        MONGOLIA,
        MONTENEGRO,
        MONTSERRAT,
        MOROCCO,
        MOZAMBIQUE,
        MYANMAR,
        NAMIBIA,
        NAURU,
        NEPAL,
        NETHERLANDS,
        NETHERLANDS_ANTILLES,
        NEW_CALEDONIA,
        NEW_ZEALAND,
        NICARAGUA,
        NIGER,
        NIGERIA,
        NIUE,
        NORFOLK_ISLAND,
        NORTHERN_MARIANA_ISLANDS,
        NORWAY,
        OMAN,
        PAKISTAN,
        PALAU,
        PALESTINIAN_TERRITORY,
        PANAMA,
        PAPUA_NEW_GUINEA,
        PARAGUAY,
        PERU,
        PHILIPPINES,
        PITCAIRN,
        POLAND,
        PORTUGAL,
        PUERTO_RICO,
        QATAR,
        REUNION,
        ROMANIA,
        RUSSIAN_FEDERATION,
        RWANDA,
        SAINTBARTHELEMY,
        SAINT_HELENA,
        SAINT_KITTS_AND_NEVIS,
        SAINT_LUCIA,
        SAINTMARTIN_FRENCH_PART,
        SAINT_PIERRE_AND_MIQUELON,
        SAINT_VINCENT_AND_GRENADINES,
        SAMOA,
        SAN_MARINO,
        SAO_TOME_AND_PRINCIPE,
        SAUDI_ARABIA,
        SENEGAL,
        SERBIA,
        SEYCHELLES,
        SIERRA_LEONE,
        SINGAPORE,
        SLOVAKIA,
        SLOVENIA,
        SOLOMON_ISLANDS,
        SOMALIA,
        SOUTH_AFRICA,
        SOUTH_GEORGIA_AND_THE_SOUTH_SANDWICH_ISLANDS,
        SOUTH_SUDAN,
        SPAIN,
        SRI_LANKA,
        SUDAN,
        SURINAME,
        SVALBARD_AND_JAN_MAYEN_ISLANDS,
        SWAZILAND,
        SWEDEN,
        SWITZERLAND,
        SYRIAN_ARAB_REPUBLIC_SYRIA,
        TAIWAN_REPUBLIC_OF_CHINA,
        TAJIKISTAN,
        TANZANIA_UNITED_REPUBLIC_OF,
        THAILAND,
        TIMORLESTE,
        TOGO,
        TOKELAU,
        TONGA,
        TRINIDAD_AND_TOBAGO,
        TUNISIA,
        TURKEY,
        TURKMENISTAN,
        TURKS_AND_CAICOS_ISLANDS,
        TUVALU,
        UGANDA,
        UKRAINE,
        UNITED_ARAB_EMIRATES,
        UNITED_KINGDOM,
        UNITED_STATES_OF_AMERICA,
        US_MINOR_OUTLYING_ISLANDS,
        URUGUAY,
        UZBEKISTAN,
        VANUATU,
        VENEZUELA_BOLIVARIAN_REPUBLIC,
        VIET_NAM,
        VIRGIN_ISLANDS_US,
        WALLIS_AND_FUTUNA_ISLANDS,
        WESTERN_SAHARA,
        YEMEN,
        ZAMBIA,
        ZIMBABWE,
        UNKNOWN,
    };

    /**
     * @brief count
     * 
     */
    inline constexpr auto count          = static_cast<std::size_t>(ID::UNKNOWN) + 1;

    /**
     * @brief defaultCountry
     * 
     */
    inline constexpr auto defaultCountry = ID::UNKNOWN;

    /**
     * @brief Data[name, alpha2, alpha3, un_number]
     * 
     */
    struct Data {
        const char *name;
        const char *alpha2;
        const char *alpha3;
        unsigned int un_number;
    };

    /**
     * @brief map[country_codes]
     * 
     */
    inline constexpr std::array<struct Data, count> map = {{
        {"Afghanistan", "AF", "AFG", 4},                       
        {"ALA Aland Islands", "AX", "ALA", 248},               
        {"Albania", "AL", "ALB", 8},                           
        {"Algeria", "DZ", "DZA", 12},                         
        {"American Samoa", "AS", "ASM", 16},                  
        {"Andorra", "AD", "AND", 20},                         
        {"Angola", "AO", "AGO", 24},                          
        {"Anguilla", "AI", "AIA", 660},                       
        {"Antarctica", "AQ", "ATA", 10},                      
        {"Antigua and Barbuda", "AG", "ATG", 28},             
        {"Argentina", "AR", "ARG", 32},                       
        {"Armenia", "AM", "ARM", 51},                         
        {"Aruba", "AW", "ABW", 533},                          
        {"Australia", "AU", "AUS", 36},                       
        {"Austria", "AT", "AUT", 40},                         
        {"Azerbaijan", "AZ", "AZE", 31},                      
        {"Bahamas", "BS", "BHS", 44},                         
        {"Bahrain", "BH", "BHR", 48},                         
        {"Bangladesh", "BD", "BGD", 50},                      
        {"Barbados", "BB", "BRB", 52},                        
        {"Belarus", "BY", "BLR", 112},                         
        {"Belgium", "BE", "BEL", 56},                          
        {"Belize", "BZ", "BLZ", 84},                           
        {"Benin", "BJ", "BEN", 204},                           
        {"Bermuda", "BM", "BMU", 60},                          
        {"Bhutan", "BT", "BTN", 64},                           
        {"Bolivia", "BO", "BOL", 68},                          
        {"Bosnia and Herzegovina", "BA", "BIH", 70},           
        {"Botswana", "BW", "BWA", 72},                         
        {"Bouvet Island", "BV", "BVT", 74},                    
        {"Brazil", "BR", "BRA", 76},                           
        {"British Virgin Islands", "VG", "VGB", 92},           
        {"British Indian Ocean Territory", "IO", "IOT", 86},   
        {"Brunei Darussalam", "BN", "BRN", 96},                
        {"Bulgaria", "BG", "BGR", 100},                        
        {"Burkina Faso", "BF", "BFA", 854},                    
        {"Burundi", "BI", "BDI", 108},                         
        {"Cambodia", "KH", "KHM", 116},                        
        {"Cameroon", "CM", "CMR", 120},                        
        {"Canada", "CA", "CAN", 124},                          
        {"Cape Verde", "CV", "CPV", 132},                      
        {"Cayman Islands", "KY", "CYM", 136},                  
        {"Central African Republic", "CF", "CAF", 140},        
        {"Chad", "TD", "TCD", 148},                            
        {"Chile", "CL", "CHL", 152},                           
        {"China", "CN", "CHN", 156},                           
        {"Hong Kong, SAR China", "HK", "HKG", 344},            
        {"Macao, SAR China", "MO", "MAC", 446},                
        {"Christmas Island", "CX", "CXR", 162},                
        {"Cocos (Keeling) Islands", "CC", "CCK", 166},         
        {"Colombia", "CO", "COL", 170},                        
        {"Comoros", "KM", "COM", 174},                        
        {"Congo (Brazzaville)", "CG", "COG", 178},             
        {"Congo, (Kinshasa)", "CD", "COD", 180},               
        {"Cook Islands", "CK", "COK", 184},                    
        {"Costa Rica", "CR", "CRI", 188},                      
        {"Côte d'Ivoire", "CI", "CIV", 384},                   
        {"Croatia", "HR", "HRV", 191},                         
        {"Cuba", "CU", "CUB", 192},                            
        {"Cyprus", "CY", "CYP", 196},                         
        {"Czech Republic", "CZ", "CZE", 203},                 
        {"Denmark", "DK", "DNK", 208},                        
        {"Djibouti", "DJ", "DJI", 262},                       
        {"Dominica", "DM", "DMA", 212},                       
        {"Dominican Republic", "DO", "DOM", 214},             
        {"Ecuador", "EC", "ECU", 218},                        
        {"Egypt", "EG", "EGY", 818},                          
        {"El Salvador", "SV", "SLV", 222},                    
        {"Equatorial Guinea", "GQ", "GNQ", 226},              
        {"Eritrea", "ER", "ERI", 232},                        
        {"Estonia", "EE", "EST", 233},                        
        {"Ethiopia", "ET", "ETH", 231},                       
        {"Falkland Islands (Malvinas)", "FK", "FLK", 238},     
        {"Faroe Islands", "FO", "FRO", 234},                   
        {"Fiji", "FJ", "FJI", 242},                            
        {"Finland", "FI", "FIN", 246},                         
        {"France", "FR", "FRA", 250},                          
        {"French Guiana", "GF", "GUF", 254},                   
        {"French Polynesia", "PF", "PYF", 258},               
        {"French Southern Territories", "TF", "ATF", 260},    
        {"Gabon", "GA", "GAB", 266},                          
        {"Gambia", "GM", "GMB", 270},                         
        {"Georgia", "GE", "GEO", 268},                         
        {"Germany", "DE", "DEU", 276},                        
        {"Ghana", "GH", "GHA", 288},                           
        {"Gibraltar", "GI", "GIB", 292},                       
        {"Greece", "GR", "GRC", 300},                          
        {"Greenland", "GL", "GRL", 304},                       
        {"Grenada", "GD", "GRD", 308},                         
        {"Guadeloupe", "GP", "GLP", 312},                      
        {"Guam", "GU", "GUM", 316},                            
        {"Guatemala", "GT", "GTM", 320},                       
        {"Guernsey", "GG", "GGY", 831},                        
        {"Guinea", "GN", "GIN", 324},                          
        {"Guinea-Bissau", "GW", "GNB", 624},                   
        {"Guyana", "GY", "GUY", 328},                          
        {"Haiti", "HT", "HTI", 332},                           
        {"Heard and Mcdonald Islands", "HM", "HMD", 334},      
        {"Holy See (Vatican City State)", "VA", "VAT", 336},   
        {"Honduras", "HN", "HND", 340},                        
        {"Hungary", "HU", "HUN", 348},                         
        {"Iceland", "IS", "ISL", 352},                         
        {"India", "IN", "IND", 356},                           
        {"Indonesia", "ID", "IDN", 360},                       
        {"Iran, Islamic Republic of", "IR", "IRN", 364},       
        {"Iraq", "IQ", "IRQ", 368},                            
        {"Ireland", "IE", "IRL", 372},                        
        {"Isle of Man", "IM", "IMN", 833},                    
        {"Israel", "IL", "ISR", 376},                         
        {"Italy", "IT", "ITA", 380},                          
        {"Jamaica", "JM", "JAM", 388},                        
        {"Japan", "JP", "JPN", 392},                          
        {"Jersey", "JE", "JEY", 832},                         
        {"Jordan", "JO", "JOR", 400},                         
        {"Kazakhstan", "KZ", "KAZ", 398},                     
        {"Kenya", "KE", "KEN", 404},                          
        {"Kiribati", "KI", "KIR", 296},                       
        {"Korea (North)", "KP", "PRK", 408},                  
        {"Korea (South)", "KR", "KOR", 410},                  
        {"Kuwait", "KW", "KWT", 414},                         
        {"Kyrgyzstan", "KG", "KGZ", 417},                     
        {"Lao PDR", "LA", "LAO", 418},                       
        {"Latvia", "LV", "LVA", 428},                        
        {"Lebanon", "LB", "LBN", 422},                       
        {"Lesotho", "LS", "LSO", 426},                       
        {"Liberia", "LR", "LBR", 430},                       
        {"Libya", "LY", "LBY", 434},                          
        {"Liechtenstein", "LI", "LIE", 438},                  
        {"Lithuania", "LT", "LTU", 440},                      
        {"Luxembourg", "LU", "LUX", 442},                     
        {"Macedonia, Republic of", "MK", "MKD", 807},         
        {"Madagascar", "MG", "MDG", 450},                     
        {"Malawi", "MW", "MWI", 454},                         
        {"Malaysia", "MY", "MYS", 458},                       
        {"Maldives", "MV", "MDV", 462},                       
        {"Mali", "ML", "MLI", 466},                           
        {"Malta", "MT", "MLT", 470},                          
        {"Marshall Islands", "MH", "MHL", 584},               
        {"Martinique", "MQ", "MTQ", 474},                     
        {"Mauritania", "MR", "MRT", 478},                    
        {"Mauritius", "MU", "MUS", 480},                      
        {"Mayotte", "YT", "MYT", 175},                        
        {"Mexico", "MX", "MEX", 484},                        
        {"Micronesia, Federated States of", "FM", "FSM", 583},
        {"Moldova", "MD", "MDA", 498},                       
        {"Monaco", "MC", "MCO", 492},                        
        {"Mongolia", "MN", "MNG", 496},                      
        {"Montenegro", "ME", "MNE", 499},                    
        {"Montserrat", "MS", "MSR", 500},                    
        {"Morocco", "MA", "MAR", 504},                       
        {"Mozambique", "MZ", "MOZ", 508},                    
        {"Myanmar", "MM", "MMR", 104},                       
        {"Namibia", "NA", "NAM", 516},                       
        {"Nauru", "NR", "NRU", 520},                        
        {"Nepal", "NP", "NPL", 524},                        
        {"Netherlands", "NL", "NLD", 528},                   
        {"Netherlands Antilles", "AN", "ANT", 530},          
        {"New Caledonia", "NC", "NCL", 540},                 
        {"New Zealand", "NZ", "NZL", 554},                   
        {"Nicaragua", "NI", "NIC", 558},                     
        {"Niger", "NE", "NER", 562},                         
        {"Nigeria", "NG", "NGA", 566},                       
        {"Niue", "NU", "NIU", 570},                          
        {"Norfolk Island", "NF", "NFK", 574},                
        {"Northern Mariana Islands", "MP", "MNP", 580},      
        {"Norway", "NO", "NOR", 578},                        
        {"Oman", "OM", "OMN", 512},                          
        {"Pakistan", "PK", "PAK", 586},                      
        {"Palau", "PW", "PLW", 585},                         
        {"Palestinian Territory", "PS", "PSE", 275},         
        {"Panama", "PA", "PAN", 591},                        
        {"Papua New Guinea", "PG", "PNG", 598},              
        {"Paraguay", "PY", "PRY", 600},                      
        {"Peru", "PE", "PER", 604},                          
        {"Philippines", "PH", "PHL", 608},                   
        {"Pitcairn", "PN", "PCN", 612},                      
        {"Poland", "PL", "POL", 616},                         
        {"Portugal", "PT", "PRT", 620},                       
        {"Puerto Rico", "PR", "PRI", 630},                    
        {"Qatar", "QA", "QAT", 634},                          
        {"Réunion", "RE", "REU", 638},                        
        {"Romania", "RO", "ROU", 642},                        
        {"Russian Federation", "RU", "RUS", 643},             
        {"Rwanda", "RW", "RWA", 646},                         
        {"Saint-Barthélemy", "BL", "BLM", 652},               
        {"Saint Helena", "SH", "SHN", 654},                   
        {"Saint Kitts and Nevis", "KN", "KNA", 659},          
        {"Saint Lucia", "LC", "LCA", 662},                    
        {"Saint-Martin (French part)", "MF", "MAF", 663},     
        {"Saint Pierre and Miquelon", "PM", "SPM", 666},      
        {"Saint Vincent and Grenadines", "VC", "VCT", 670},   
        {"Samoa", "WS", "WSM", 882},                          
        {"San Marino", "SM", "SMR", 674},                     
        {"Sao Tome and Principe", "ST", "STP", 678},          
        {"Saudi Arabia", "SA", "SAU", 682},                   
        {"Senegal", "SN", "SEN", 686},                        
        {"Serbia", "RS", "SRB", 688},                         
        {"Seychelles", "SC", "SYC", 690},                     
        {"Sierra Leone", "SL", "SLE", 694},                   
        {"Singapore", "SG", "SGP", 702},                      
        {"Slovakia", "SK", "SVK", 703},                       
        {"Slovenia", "SI", "SVN", 705},                       
        {"Solomon Islands", "SB", "SLB", 90},                 
        {"Somalia", "SO", "SOM", 706},                        
        {"South Africa", "ZA", "ZAF", 710},                    
        {"South Georgia and the South Sandwich Islands",
         "GS",
         "SGS",
         239},                                                 
        {"South Sudan", "SS", "SSD", 728},                     
        {"Spain", "ES", "ESP", 724},                           
        {"Sri Lanka", "LK", "LKA", 144},                       
        {"Sudan", "SD", "SDN", 736},                           
        {"Suriname", "SR", "SUR", 740},                        
        {"Svalbard and Jan Mayen Islands", "SJ", "SJM", 744},  
        {"Swaziland", "SZ", "SWZ", 748},                       
        {"Sweden", "SE", "SWE", 752},                          
        {"Switzerland", "CH", "CHE", 756},                     
        {"Syrian Arab Republic (Syria)", "SY", "SYR", 760},    
        {"Taiwan, Republic of China", "TW", "TWN", 158},       
        {"Tajikistan", "TJ", "TJK", 762},                      
        {"Tanzania, United Republic of", "TZ", "TZA", 834},     
        {"Thailand", "TH", "THA", 764},                         
        {"Timor-Leste", "TL", "TLS", 626},                      
        {"Togo", "TG", "TGO", 768},                             
        {"Tokelau", "TK", "TKL", 772},                          
        {"Tonga", "TO", "TON", 776},                            
        {"Trinidad and Tobago", "TT", "TTO", 780},             
        {"Tunisia", "TN", "TUN", 788},                          
        {"Turkey", "TR", "TUR", 792},                           
        {"Turkmenistan", "TM", "TKM", 795},                     
        {"Turks and Caicos Islands", "TC", "TCA", 796},         
        {"Tuvalu", "TV", "TUV", 798},                           
        {"Uganda", "UG", "UGA", 800},                           
        {"Ukraine", "UA", "UKR", 804},                          
        {"United Arab Emirates", "AE", "ARE", 784},             
        {"United Kingdom", "GB", "GBR", 826},                  
        {"United States of America", "US", "USA", 840},        
        {"US Minor Outlying Islands", "UM", "UMI", 581},        
        {"Uruguay", "UY", "URY", 858},                          
        {"Uzbekistan", "UZ", "UZB", 860},                      
        {"Vanuatu", "VU", "VUT", 548},                         
        {"Venezuela (Bolivarian Republic)", "VE", "VEN", 862}, 
        {"Viet Nam", "VN", "VNM", 704},                        
        {"Virgin Islands, US", "VI", "VIR", 850},              
        {"Wallis and Futuna Islands", "WF", "WLF", 876},       
        {"Western Sahara", "EH", "ESH", 732},                  
        {"Yemen", "YE", "YEM", 887},                           
        {"Zambia", "ZM", "ZMB", 894},                          
        {"Zimbabwe", "ZW", "ZWE", 716},                        
        {"Unknown", "ZZ", "ZZZ", 0},                           
    }};

    
    /**
     * @brief Get the Name object
     * 
     * @param id 
     * @return constexpr const char* 
     */
    constexpr const char *getName(ID id) {
        return map[static_cast<std::size_t>(id)].name;
    }

    /**
     * @brief Get the Alpha2 Code object
     * 
     * @param id 
     * @return constexpr const char* 
     */
    constexpr const char *getAlpha2Code(ID id) {
        return map[static_cast<std::size_t>(id)].alpha2;
    }

    /**
     * @brief Get the Alpha3 Code object
     * 
     * @param id 
     * @return constexpr const char* 
     */
    constexpr const char *getAlpha3Code(ID id) {
        return map[static_cast<std::size_t>(id)].alpha3;
    }

    /**
     * @brief getUNNumber
     * 
     * @param id 
     * @return constexpr unsigned int 
     */
    constexpr unsigned int getUNnumber(ID id) {
        return map[static_cast<std::size_t>(id)].un_number;
    }

    /**
     * @brief Get the Id By Alpha2 Code object
     * 
     * @param alpha2Code 
     * @return ID 
     */
    inline ID getIdByAlpha2Code(const std::string &alpha2Code) {
        for (std::size_t i = 0; i < count; i++) {
            if (std::string(map[i].alpha2) == alpha2Code) {
                return static_cast<ID>(i);
            }
        }

        return ID::UNKNOWN;
    }

} 