#include "backend.h"

//View models
#include "tournamentsmodel.h"
#include "tournamentviewmodel.h"

//PGN reader
#include "PGNGameCollection.h"
#include <sstream>
#include <iostream>

//Qt
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>

//Chess24
#include "chess24.h"
#include "chess24websocket.h"
#include "preparechess24ws.h"
#include "chess24messages.h"
#include "wsrequest.h"

Backend::Backend(QObject *parent,TournamentsModel *tm):
    QObject(parent), c24ws(parent,qnam), prepc24(parent,qnam), tm(tm)
{
    /*//Chess24 c24;
    QUrl pgnurl("http://theweekinchess.com/assets/files/pgn/chgbr17.pgn");

    std::cout << "Getting pgn" << std::endl;
    QNetworkRequest request(pgnurl);
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla Firefox");
    reply = qnam.get(request);
    connect(reply,&QNetworkReply::finished,this,&Backend::httpFinished);**/


    /*foreach(auto o,engine.rootObjects()){
        qDebug() << o->objectName();
    }*/

    /*QQmlComponent loginComponent(&engine,QUrl(QStringLiteral("qrc:/LoginDialog.qml")));
    foreach(auto error,loginComponent.errors()){
        qDebug() << error;
    }

    QObject *loginObject = loginComponent.create();
    loginObject->setParent(engine.rootObjects().first());
    QQuickItem *loginDialog = qobject_cast<QQuickItem*>(loginObject);
    loginDialog->setParentItem(engine.rootObjects().first());
    qDebug() << engine.rootObjects().first()->objectName();
    qDebug() << loginObject->parent();
    connect(loginObject,SIGNAL(accepted()),this,SLOT(loginAccepted()));


    foreach(auto c, engine.children().first()->children()){
        qDebug() << c->objectName();
    }*/

    //c24.login("test","test");
    //connect(&c24,&Chess24::loginResult,this,&Backend::loggedIn);

    /*const QString dat = "[[\"grand-chess-tour-saint-louis-rapid-and-blitz-2017\",\"spanish-honor-league-2017\",\"sants-open-2017\",\"korchnoi-memorial-2017\",\"european-senior-championships-2017\",\"european-youth-team-championship-2017\",\"european-girls-youth-team-championship-2017\",\"iranian-chess-festival-2017\",\"armenian-championship-1st-league-2017\",\"chinese-league-2017\",\"abu-dhabi-2017\",\"2017-county-championship-final\",\"polish-ekstraliga-2017\",\"warakomska-memorial-2017-a\",\"washington-international-2017\",\"wroclaw-open-2017\",\"spanish-championship-2017-rapid\",\"gelfand-inarkiev-match-2017\",\"ukcc-terafinal-2017\",\"rtu-open-2017\",\"mislata-open-2017\",\"arad-open-2017\",\"spanish-championship-2017-blitz\",\"vlissingen-open-2017\",\"2nd-spanish-league-2017\",\"grand-chess-tour-sinquefield-cup-2017\",\"prague-open-2017\",\"ding-liren-vs-giri-2017\",\"uae-championship-2017\",\"aslanov-memorial-2017-blitz\",\"aslanov-memorial-2017-rapid\",\"aslanov-memorial-2017\",\"us-open-2017\",\"british-chess-championship-2017\",\"cellavision-chess-cup-2017\",\"espana-sub18rapid-2017\",\"erfurt-girls-grand-prix-2017\",\"irish-championship-2017\",\"german-masters-2017\",\"dresden-open-2017\",\"almansa-open-2017\",\"turkish-super-league-2017\",\"nq-open-2017\",\"espana-sub18-2017\",\"13th-gazi-heraklion-open-2017\",\"erfurt-young-masters-2017\",\"erfurt-womens-open-2017\",\"kavala-open-2017\",\"erfurt-ladies-champions-match-2017\",\"condigne-dutch-open-2017\",\"biel-grandmaster-tournament-2017\",\"amazon-vs-microsoft-rivalry-match-2017\",\"german-masters-2017-challenger\",\"espana-sub16rapid-2017\",\"german-womens-rapid-championship-2017\",\"xtracon-open-2017\",\"sueca-open-2017\",\"rgcc-open-2017\",\"travnik-open-2017\",\"sitges-open-2017\",\"andorra-open-2017\",\"match-of-the-millennials-2017-u17\",\"match-of-the-millennials-2017-u14\",\"czech-open-2017\",\"espana-sub161-2017\",\"espana-sub16-2017\",\"astana-open-2017\",\"romanian-youth-team-championships-2017-rapid\",\"romanian-youth-team-championships-2017\",\"paleochora-open-2017\",\"yu-yangyi-vs-grischuk\",\"biel-accentus-rapid-tournament-2017\",\"scottish-championship-2017\",\"portuguese-league-2017\",\"dundee-150\",\"espana-sub14rapid-2017\",\"dortmund-sparkassen-chess-meeting-2017\",\"leiden-2017\",\"heart-of-finland-2017\",\"espana-sub14-2017\",\"south-african-open-championship-2017\",\"lake-sevan-2017\",\"xativa-open-2017\",\"sant-marti-open-2017\",\"swiss-championship-2017\",\"czech-rapid-championship-2017\",\"aguila2017\",\"faroes-open-2017\",\"entrenadores2017\",\"6th-idaion-andron-2017\",\"bergamo-open-2017\",\"espana-sub12rapid-2017\",\"polugaevsky-memorial-2017\",\"espana-sub12-2017\",\"benasque-open-2017\",\"womens-russian-higher-league-2017\",\"russian-higher-league-2017\",\"espana-sub10-rapidas-2017\",\"finnish-championship-2017-rapid\",\"norwegian-championship-elite-2017\",\"espana-sub10-2017\",\"greek-team-championship-2017\",\"lodz-championship-2017\",\"cracow-championship-2017\",\"world-open-2017\",\"montcada-open-2017\",\"dutch-womens-championship-2017\",\"dutch-championship-2017\",\"nordic-championship-2017\",\"greek-youth-2017-open-u18\",\"world-chess-legends-2017\",\"fisca-2017-u14\",\"fisca-2017-open\",\"german-championship-2017\",\"closed-gm-ceske-budejovice-2017\",\"gens-una-sumus-2017\",\"karen-asrian-memorial-2017\",\"mykolaiv-chess-fest-2017-open\",\"world-womens-team-championship-2017\",\"world-team-championship-2017\",\"paris-grand-chess-tour-2017\",\"ey-chess-2017\",\"french-womens-league-2017-finals\",\"carlos-i-open-2017\",\"espanasub14equipos\",\"silver-lake-open-2017\",\"baltic-zonal-liepaja-stage-2017\",\"mykolaiv-chess-fest-2017-rapid\",\"voronezh-masters-2017\",\"american-continental-championship-2017\",\"finnish-womens-championship-2017\",\"finnish-championship-2017\",\"german-womens-state-team-championship-2017\",\"teplice-open-2017\",\"cesme-open-2017\",\"ivanchuk-navara-2017\",\"chinese-womens-team-championship-2017\",\"chinese-team-championship-2017\",\"madrid-championship-2017\",\"finnish-youth-championship-2017-u8-u10\",\"finnish-youth-championship-2017-u12-u20\",\"european-championship-2017\",\"lublin-union-memorial-2017\",\"deutsche-jugendeinzelmeisterschaft-2017-u18w\",\"deutsche-jugendeinzelmeisterschaft-2017-u18\",\"deutsche-jugendeinzelmeisterschaft-2017-u16w\",\"deutsche-jugendeinzelmeisterschaft-2017-u16\",\"deutsche-jugendeinzelmeisterschaft-2017-u14w\",\"deutsche-jugendeinzelmeisterschaft-2017-u14\",\"deutsche-jugendeinzelmeisterschaft-2017-u12w\",\"deutsche-jugendeinzelmeisterschaft-2017-u12\",\"deutsche-jugendeinzelmeisterschaft-2017-u10w\",\"deutsche-jugendeinzelmeisterschaft-2017-u10\",\"deutsche-jugendeinzelmeisterschaft-2017-odjmb\",\"deutsche-jugendeinzelmeisterschaft-2017-odjma\",\"39th-nezhmetdinov-memorial-2017\",\"52nd-capablanca-memorial-2017\",\"altibox-norway-chess-2017-blitz\",\"limburg-open-2017\",\"zira-open-2017-rapid\",\"fide-world-cadet-championship-2017-blitz\",\"chinese-womens-masters-2017\",\"fide-world-cadet-championship-2017-rapid\",\"chicago-open-2017\",\"ayr-chess-congress-2017\",\"top-12-french-teams-2017\",\"orlova-open-2017\",\"36th-zalakaros-open-2017\",\"spring-chess-classic-2017-c\",\"spring-chess-classic-2017-b\",\"spring-chess-classic-2017-a\",\"circuito-panameno-de-ajedrez-2017-may\",\"attica-cup-final-4-2017\",\"polish-hungarian-friendship-2017\",\"fide-grand-prix-2017-moscow\",\"6th-llucmajor-open-2017\",\"asian-womens-continental-chess-championship-2017-blitz\",\"asian-continental-chess-championship-2017-blitz\",\"icelandic-chess-championship-2017\",\"mukachevo-2017-im\",\"asian-womens-continental-chess-championship-2017\",\"asian-continental-chess-championship-2017\",\"47th-bosna-2017\",\"tepe-sigeman-chess-2017\",\"german-team-cup-finals-2017\",\"croatian-womens-championship-2017\",\"3rd-fluchess-2017\",\"nakhchivan-open-2017\",\"russian-team-championship-women-2017\",\"russian-team-championship-premier-2017\",\"russian-team-championship-hl-2017\",\"asian-junior-championship-2017\",\"asian-girls-junior-championship-2017\",\"fide-zonal-arg-2017\",\"french-womens-league-2017-b\",\"french-womens-league-2017-a\",\"essth-x-cup-final-4-2017\",\"czech-championship-2017\",\"catalunya2017\",\"croatian-championship-2017\",\"mexican-university-championship-2017\",\"chinese-womens-championship-2017\",\"chinese-championship-2017\",\"world-senior-team-championship-s65-2017\",\"world-senior-team-championship-s50-2017\",\"4ncl-division-1-finals-2016-2017\",\"womens-bundesliga-2016-2017\",\"schachbundesliga-2016-2017\",\"pss-capablanca-2017-im\",\"pss-capablanca-2017-amateur-d\",\"pss-capablanca-2017-amateur-c\",\"pss-capablanca-2017-amateur-b\",\"pss-capablanca-2017-amateur-a\",\"49th-cis-master-2017\",\"10th-cis-femminile-2017\",\"24th-space-coast-open-2017\",\"shamkir-chess-gashimov-memorial-2017\",\"cretan-cup-final-4-2017\",\"russian-junior-girls-championship-2017\",\"russian-junior-championship-2017\",\"magistral-memorial-jorge-szmetan-2017\",\"poikovsky-2017\",\"1st-brics-chess-masters-2017\",\"20th-heraklion-open-2017\",\"1st-heraklion-im-norm-2017\",\"1st-heraklion-gm-norm-2017\",\"pss-open-2017-blitz\",\"pss-open-2017-rapid\",\"oberliga-wuerttemberg-2017\",\"10th-triandria-2017\",\"european-womens-championship-2017\",\"kazakhstan-championship-2017\",\"kortchnoi-zurich-chess-challenge-2017\",\"grenke-chess-open-2017\",\"kortchnoi-open-zurich-2017\",\"iran-juniors-2017\",\"doeberl-cup-2017\",\"mexican-open-2017\",\"la-roda-2017\",\"danish-championship-2017\",\"tv2-fagernes-im-2017\",\"tv2-fagernes-gm-2017\",\"17th-bangkok-chess-open-2017\",\"pedro-lezcano-montalvo-memorial-2017\",\"45th-san-sebastian-open-2017\",\"kortchnoi-zurich-chess-challenge-2017-blitz\",\"zurich-skvortsov-anand-2017\",\"19th-dubai-open-2017\",\"kazakhstan-womens-championship-2017\",\"us-championship-2017\",\"us-womens-championship-2017\",\"moscow-womens-championship-2017\",\"moscow-championship-2017\",\"wlc-weekend-tournament-2017\",\"polar-capital-open-2017\",\"german-womens-championship-2017\",\"karpos-open-2017\",\"hou-yifan-ivanchuk-2017\",\"uruguayan-championship-2017\",\"romanian-womens-championship-2017\",\"romanian-championship-2017\",\"shenzhen-chess-masters-2017\",\"cebanenko-memorial-rapid-2017\",\"copa-norberto-laporta-2017\",\"sharjah-masters-2017\",\"2nd-fluchess-2017\",\"serbian-championship-2017\",\"polish-womens-championship-2017\",\"polish-championship-2017\",\"90-st-petersburg-championship-2017\",\"baltic-zonal-2017\",\"gaprindashvili-sveshnikov-2017\",\"us-college-final-four-2017\",\"budapest-spring-open-2017\",\"lviv-tradition-group-b-2017-march\",\"lviv-tradition-group-a-2017-march\",\"georgian-womens-championship-2017\",\"winter-chess-classic-2017-group-b\",\"winter-chess-classic-2017-group-a\",\"paz-de-ziganda-2017\",\"greek-womens-championship-2017\",\"7th-hdbank-masters-2017\",\"azerbaijan-championship-2017\",\"iranian-championship-2017\",\"circuito-panameno-de-ajedrez-2017\",\"german-team-cup-intermediate-stage-2017\",\"greek-championship-2017\",\"stockholm-chess-challenge-2017\",\"iranian-womens-championship-2017\",\"oxford-cambridge-varsity-match-2017\",\"arzum-turkish-womens-championship-2017\",\"2nd-china-india-summit-2017\",\"tal-memorial-2017-rapid\",\"9th-batavia-chess-tournament-2017\",\"norwegian-league-2016-2017\",\"4ncl-division-1-pool-b-2016-2017\",\"4ncl-division-1-pool-a-2016-2017\",\"lund-limhamn-2017\",\"granda-alaciega\",\"fide-womens-world-championship-2017\",\"aeroflot-open-2017\",\"Chile-final-2017\",\"fide-grand-prix-2017-sharjah\",\"vardaris-open-2017\",\"matsk-weekender-2017\",\"british-universities-2017\",\"kragero-resort-chess-2017\",\"graz-open-2017\",\"9th-batavia-chess-tournament-2017-blitz\",\"st-louis-winter-2017-IM\",\"st-louis-winter-2017\",\"brazilian-womens-championship-2017\",\"bunratty-masters-2017\",\"lienz-open-2017\",\"azerbaijan-womens-championship-2017\",\"marcel-duchamp-cup-2017\",\"83rd-brazilian-championship-2017\",\"portugal-open-2017\",\"1st-fluchess-2017\",\"1st-heraklion-weekender-2017\",\"womens-moscow-open-2017\",\"moscow-open-2017\",\"turkish-cup-2017\",\"tradewise-gibraltar-masters-2017\",\"tradewise-gibraltar-challengers-2-2017\",\"3rd-yerevan-mayors-cup-2017\",\"tata-steel-masters-2017\",\"tata-steel-challengers-2017\",\"lviv-tradition-2017-january\",\"tradewise-gibraltar-challengers-2017\",\"15th-khazar-cup-2017\",\"chennai-2017\",\"french-university-rapid-championship-2017\",\"kings-and-queens-2017-january\",\"77th-armenian-championship-2017\",\"belarusian-championship-2017\",\"margaryan-memorial-2017\",\"delhi-open-2017\",\"zicosur2017\",\"keres-memorial-2017\",\"australian-open-2017\",\"19th-heraklion-open-2017\",\"bosnjaci-open-2017\",\"polish-rapid-championship-2017\",\"roquetas-open-2017\",\"basel-masters-2017\",\"womens-greek-championship-2017-u20\",\"greek-championship-2017-u20\",\"chilefem2017\",\"bad-zwesten-open-2017\",\"hastings-masters-2016-2017\",\"46th-rilton-cup\",\"roquetas-open-2016\",\"world-blitz-championship-2016\",\"womens-world-blitz-championship-2016\",\"groningen-2016\",\"dvm-2016-u16\",\"dvm-2016-u20w\",\"dvm-2016-u20\",\"dvm-2016-u14w\",\"dvm-2016-u14\",\"world-rapid-championship-2016\",\"womens-world-rapid-championship-2016\",\"solidarity-day-rapid-2016\",\"sitges2016\",\"solidarity-day-open-2016\",\"rapport-wei-yi-2016\",\"nutcracker-2016\",\"super-rapidplay-2016\",\"london-chess-classic-2016\",\"european-blitz-2016\",\"greek-youth-team-championship-2016\",\"australian-masters-2016\",\"ukrainian-womens-championship-2016\",\"ukrainian-championship-2016\",\"zadar-open-2016\",\"british-knockout-championship-2016\",\"london-chess-classic-open-2016\",\"european-rapid-2016\",\"hainan-open-2016\",\"baltic-zonal-2016\",\"israel-womens-championship-2016\",\"israel-championship-2016\",\"paraguay2016\",\"russian-cup-final-2016\",\"sub12\",\"sub18\",\"world-school-championship-2016\",\"russian-womens-cup-final-2016\",\"sub2300bali\",\"bali2016\",\"rome-city-open-2016\",\"australian-young-masters-2016\",\"london-pro-biz-cup-2016\",\"chinese-league-2016\",\"tcec-2016\",\"acl-im-tournament-2016\",\"world-blitz-record-2016\",\"russian-rapid-gp-final-2016\",\"turkish-championship-2016\",\"wroclaw-wgm-2016\",\"khanty-wgp-2016\",\"cearense-championship-2016\",\"world-senior-2016-65\",\"world-senior-2016-50\",\"womens-world-senior-2016\",\"houyifan-kramnik-2016\",\"indian-championship-2016\",\"krulich-cup-2016\",\"runavik-open-2016\",\"romanian-superliga-2016\",\"yugra-governors-cup-2016\",\"asian-amateur-2016-women\",\"asian-amateur-2016-open\",\"champions-showdown-2016\",\"azerbaijan-flag-day-rapid-2016\",\"womens-european-club-cup-2016\",\"european-club-cup-2016\",\"Suleymanpasa-gm-tournament-2016\",\"day-of-the-flag-2016-blitz\",\"ariadne-masters-2016-rapid\",\"armenian-championship-1st-league\",\"figueira-da-foz-open-2016\",\"20th-open-bavarian-championship-2016\",\"uppsala-youth-champions-2016\",\"paytakht-cup-2016\",\"deizisau-herbstopen-2016\",\"corsica-knockout-2016\",\"cne-dutch-womens-rapid-championship-2016\",\"cne-dutch-rapid-championship-2016\",\"nordic-championship-2016\",\"fide-world-cadets-championship-2016-o8\",\"fide-world-cadets-championship-2016-g8\",\"fide-world-cadets-championship-2016-g12\",\"fide-world-cadets-championship-2016-g10\",\"karpov-trophy-ko-2016\",\"magistralbarcelona2016\",\"karpov-trophy-2016\",\"corsica-open-2016\",\"russian-womens-championship-2016\",\"russian-championship-2016\",\"portuguese-supercup-2016\",\"korbach-open-2016\",\"haarlem-masters-2016\",\"chigorin-memorial-2016\",\"spice-cup-2016\",\"hou-yifan-short-2016\",\"hoogeveen-open-2016\",\"amir-temur-memorial-2016\",\"azerbaijan-independence-blitz-2016\",\"veteranosespana2016\",\"arco-open-2016\",\"azerbaijan-independence-rapid-2016\",\"almaty-open-2016\",\"millionaire-chess-2016-monday\",\"1st-yerevan-open-2016\",\"millionaire-chess-2016-playoff\",\"millionaire-chess-2016-open\",\"portuguese-championship-2016\",\"isle-of-man-2016\",\"swiss-team-championship-2016\",\"polish-extraliga-2016\",\"karpov-timman-2016\",\"russian-rapid-team-championship-2016\",\"russian-womens-rapid-team-championship-2016\",\"oslo-chess-festival-2016\",\"tal-memorial-2016\",\"dlm-2016\",\"russian-womens-blitz-championship-2016\",\"russian-blitz-championship-2016\",\"hammer-navara-match-2016\",\"russian-womens-rapid-championship-2016\",\"russian-rapid-championship-2016\",\"fide-world-youth-championships-2016\",\"serbian-league-2016\",\"spanish-team-championship-2016\",\"hammer-simul-2016\",\"georgian-club-championship-2016\",\"baku-open-2016\",\"2nd-norman-chess-festival-2016\",\"tal-memorial-2016-blitz\",\"play-magnus-live-challenge-2016\",\"vera-menchik-memorial-2016\",\"dziedzic-memorial-2016\",\"sissa-en-passant-2-2016\",\"42nd-chess-olympiad-baku-2016-women\",\"42nd-chess-olympiad-baku-2016-open\",\"18th-heraklion-open-2016\",\"luesdorf-rapid-2016\",\"pyramids-cup-2016\",\"24th-nikaia-open-2016\",\"isla-de-la-palma-2016\",\"avicenna-open-2016\",\"abu-dhabi-2016\",\"st-petersburg-rapid-2016\",\"rubinstein-memorial-2016\",\"zira-open-blitz-2016\",\"dutch-championship-2016\",\"2016-dutch-womens-championship\",\"sants-open-2016\",\"c-open-rubinstein-memorial-2016\",\"zira-open-rapid-2016\",\"euro-youth-2016-u8\",\"euro-youth-2016-u18\",\"euro-youth-2016-u16\",\"euro-youth-2016-u14\",\"euro-youth-2016-u12\",\"euro-youth-2016-u10\",\"spanish-premier-2016\",\"27th-nato-championship-2016\",\"20th-vienna-open-2016\",\"french-womens-championship-2016\",\"french-championship-2016\",\"world-junior-championship-2016\",\"world-girls-junior-championship-2016\",\"portuguese-league-2016\",\"20162ceclubespana\",\"2016-manhem-gm\",\"washington-international-2016\",\"sinquefield-cup-2016-ultimate-moves\",\"poznan-chess-festival-2016\",\"botvinnik-cup-2016\",\"espanarapid2016\",\"grand-chess-tour-sinquefield-cup-2016\",\"rtu-open-2016\",\"hz-2016\",\"espana2016\",\"arad-open-2016\",\"Andorra2016\",\"ramchess-aydin-guseinov-memorial-blitz-2016\",\"ramchess-aydin-guseinov-memorial-rapid-2016\",\"ramchess-aydin-guseinov-memorial-2016\",\"2016-german-womens-masters\",\"kavala-open-2016\",\"british-chess-championships-2016\",\"turkish-league-2016\",\"12th-gazi-heraklion-open-2016\",\"trinidad-and-tobago-international-masters-2016\",\"condigne-dutch-open-2016\",\"2016-chinese-team-championship\",\"calgary-international-2016\",\"mvl-svidler-2016\",\"poikovsky-2016\",\"8th-vellmar-chess-days-2016\",\"panamerican-youth-2016\",\"xtracon-open-2016\",\"biel-blitz-2016\",\"czech-open-gm-2016\",\"sub18espana2016\",\"world-youth-u16-olympiad\",\"paleochora-open-2016\",\"lake-sevan-2016\",\"14th-dubai-juniors-2016\",\"swedish-championships-2016\",\"leiden-2016\",\"ashley-vs-rakotomaharo\",\"9th-bilbao-masters-2016\",\"sub16espana2016\",\"gelfand-inarkiev-match-2016\",\"ding-liren-vs-grischuk\",\"sant-marti-open-2016\",\"czech-rapid-open-2016\",\"hancheng-silk-road-2016\",\"stars-cup-iran-2016\",\"5th-idaion-andron-2016\",\"usjunior2016\",\"redpath-mining-millionaire-chess-open-top8-2016\",\"english-womens-championships-2016\",\"redpath-mining-millionaire-chess-open-2016\",\"bergamo-open-elite-2016\",\"dortmund-sparkassen-chess-meeting-2016\",\"spachess-2016\",\"asian-schools-2016-open-u17\",\"asian-schools-2016-girls-u17\",\"danzhou-supergm-2016\",\"girls-european-u18-team-championship-2016\",\"european-u18-team-championship-2016\",\"norwegian-championship-elite-2016\",\"metlife-najdorf-2016\",\"sub14espana2016\",\"Benasque2016\",\"swiss-championship-2016\",\"gideon-japhet-2016\",\"chengdu-fide-womens-grand-prix-2016\",\"polugaevsky-memorial-2016\",\"qualifier-dutch-championships-2016\",\"greek-league-2016\",\"sub12espana2016\",\"usic-chess-championship-2016\",\"firouzja-maghsoodloo-match\",\"firouzja-maghsoodloo-blitz\",\"world-open-2016\",\"greek-team-cup-final-2016\",\"world-senior-team-65-2016\",\"world-senior-team-50-2016\",\"greek-youth-2016-open-under-16\",\"3rd-francophonie-rapid-championship-2016\",\"vidmar-memorial-2016\",\"county-championship-final-2016\",\"sub10espana2016\",\"womens-russian-higher-league-2016\",\"mens-russian-higher-league-2016\",\"porticcio-open-2016\",\"us-girls-junior-championship-2016\",\"mitropa-cup-2016-women\",\"mitropa-cup-2016\",\"edmonton-international-2016\",\"sub8espana2016\",\"silver-lake-open-2016\",\"karen-asrian-memorial-2016\",\"your-next-move-grand-chess-tour-2016\",\"alekhine-memorial-2016\",\"capablanca-memorial-premier-2016\",\"capablanca-memorial-elite-2016\",\"eurasian-blitz-chess-cup-2016\",\"teplice-open-2016\",\"forni-di-sopra-2016\",\"lublin-grandmaster-tournament-2016\",\"your-next-move-simul-2016\",\"russia-china-2016\",\"65-euro-senior-2016\",\"50-euro-senior-2016\",\"paris-grand-chess-tour-2016\",\"iceland-chess-championship-2016\",\"ortisei-2016\",\"czech-chess-trophy-2016\",\"top-12-french-teams-2016\",\"european-individual-womens-championship-2016\",\"american-continental-2016\",\"neoclassicalopen2016\",\"shamkir-chess-gashimov-memorial-2016\",\"Neoclassical2016\",\"trade-and-industry-meets-the-suburbs-2016\",\"asian-womens-continental-2016\",\"asian-continental-2016\",\"kasparov-simultaneous-moenchengladbach\",\"kurnosov-memorial-rapid2016\",\"nuuk-2016\",\"european-individual-championship-2016\",\"short-gretarsson-2016\",\"djem-2016-u18w\",\"djem-2016-u18\",\"blindfold-king-timur-gareyev-stockholm\",\"nakhchivan-open-2016\",\"bpb-limburg-open-2016\",\"crete-cup-2016\",\"Llucmajor2016\",\"kazakhstan-championship-2016\",\"kazakhstan-womens-championship-2016\",\"russian-womens-team-championship-2016\",\"russian-team-championship-2016\",\"russian-higher-league-teams-2016\",\"heraklion-open-2016\",\"polish-hungarian-friendship-2016\",\"hasselbacken-open-2016\",\"wunsiedel-2016-meister\",\"ding-liren-vs-wesley-so-2016\",\"poland-hungary-match-2016\",\"capablanca-chess-festival-im-tournament\",\"fide-womens-grand-prix-batumi-2016\",\"german-club-cup-2016\",\"italian-team-championship-2016\",\"ultimate-blitz-challenge-garry-kasparov\",\"norway-chess-2016\",\"chinese-championship-2016\",\"world-amateur-championship-u2300-2016\",\"world-amateur-championship-u2000-2016\",\"world-amateur-championship-u1700-2016\",\"european-senior-team-championships-s65-2016\",\"european-senior-team-championships-s50-2016\",\"pss-blitz-2016\",\"norway-sweden-2016\",\"schachbundesliga20152016\",\"dubai-open-2016\",\"serbian-championship-2016\",\"norway-chess-blitz-2016\",\"tht-Extraliga-2016\",\"bangkok-open-2016\",\"polar-capital-jersey-open\",\"small-nations-2016\",\"polish-womens-championship-2016\",\"polish-championship-2016\",\"karpos-open-2016\",\"asian-nations-womens-cup-2016\",\"asian-nations-cup-2016\",\"us-college-final-four-2016\",\"grenke-chess-open-2016\",\"candidates-2016\",\"doeberl-cup-2016\",\"laroda-2016\",\"altibox-norway-chess-qualifier-2016\",\"pfaelzischer-schachkongress-2016\",\"azerbaijan-championship-women-2016\",\"azerbaijan-championship-open-2016\",\"ey-chess-2016\",\"ziganda-2016\",\"4ncl-division-1b-2015\",\"4ncl-division-1a-2015\",\"reykjavik-open-2016\",\"women-wcc-2016\",\"ballarat-begonia-open-2016\",\"ramchess-bahar-2016\",\"stockholm-chess-challange-2016\",\"hdbank-open-2016\",\"aeroflot-open-2016\",\"norwegian-premiere-league-2016\",\"bad-woerishofen-2016-senior\",\"bad-woerishofen-2016-a-open\",\"mind-games-2016-basque\",\"mind-games-2016-women-blitz\",\"mind-games-2016-blitz\",\"continental-championship-2016\",\"batavia-tournament-2016\",\"mind-games-2016-women-rapid\",\"mind-games-2016-rapid\",\"tehran-grand-prix-2016\",\"british-university-championship-2016\",\"Cappelle-2016\",\"batavia-blitz-tournament-2016\",\"zurich-chess-challenge-2016\",\"zurich-gelfand-morozevich-match-2016\",\"zurich-chess-challenge-2016-blitz\",\"houyifan-henriquez\",\"moscow-open-2016\",\"gibraltar-masters-2016\",\"tata-steel-chess-masters-2016\",\"tata-steel-chess-challengers-2016\",\"gran-prix-peru-2016\",\"french-university-chess-championship-2016\",\"armenian-championship-2016\",\"brazilch2016\",\"delhi-open-2016\",\"magnus-carlsen-london-blindfold-simul-2016\",\"australian-championship-2016\",\"zicosur-2016\",\"keres-memorial-2016\",\"new-zealand-open-2016\",\"roquetas-2016\",\"chess-duel-sevian-vs-grandelius\",\"hastings-chess-congress-2015\",\"rilton-cup-2015\",\"lorca-2015\",\"al-ain-classic-2015\",\"zurich-christmas-2015\",\"qatar-masters-open-2015\",\"nutcracker-2015\",\"torre-memorial-2015\",\"ukrainian-champ-2015\",\"london-chess-classic-2015\",\"super-rapidplay-open-2015\",\"Mens-Russian-Cup-Final-2015\",\"london-fide-open-2015\",\"german-championship-2015\",\"womens-russian-cup-final-2015\",\"chinese-league-2015\",\"acp-masters-2015b\",\"acp-masters-2015a\",\"yugra-2015\",\"china-kings-2015\",\"central-asia-cup-2015\",\"european-team-championship-women-2015\",\"european-team-championship-open-2015\",\"showdown-saint-louis-2015\",\"edmonton-invitational-2015\",\"barcelona-chess-masters-2015\",\"elitserien-2015\",\"bad-wiessee-2015\",\"fide-world-youth-cadet-2015-open-u18\",\"fide-world-youth-cadet-2015-open-u16\",\"fide-world-youth-cadet-2015-open-u14\",\"fide-world-youth-cadet-2015-open-u12\",\"fide-world-youth-cadet-2015-open-u10\",\"fide-world-youth-cadet-2015-girls-u18\",\"fide-world-youth-cadet-2015-girls-u16\",\"fide-world-youth-cadet-2015-girls-u14\",\"fide-world-youth-cadet-2015-girls-u12\",\"fide-world-youth-cadet-2015-girls-u10\",\"iberoamerican-championship-2015\",\"bilbao-masters-final-2015\",\"uppsala-young-champions-2015\",\"european-club-cup-women-2015\",\"european-club-cup-2015\",\"hoogeveen-van-foreest-timman-2015\",\"hoogeveen-open-2015\",\"hoogeveen-kazarian-guramishvili-2015\",\"hoogeveen-basque-van-den-doel-reinderman-2015\",\"highlander-cup-2015\",\"monte-carlo-grand-prix-2015\",\"fide-world-blitz-championship-2015\",\"millionaire-chess-2015-open-knockout\",\"millionaire-chess-2015-u2550-knockout\",\"millionaire-chess-2015-u2400-knockout\",\"millionaire-chess-2015-u2200-knockout\",\"millionaire-chess-2015-u2000-knockout\",\"millionaire-chess-2015-u1800-knockout\",\"millionaire-chess-2015-u1600-knockout\",\"millionaire-chess-2015-u1400-knockout\",\"millionaire-chess-2015-u1200-knockout\",\"fide-world-rapid-championship-2015\",\"millionaire-chess-2015-tiebreak\",\"millionaire-chess-2015-open\",\"millionaire-chess-2015-under\",\"pokerstars-isle-of-man-2015\",\"poikovsky-2015\",\"akobian-troff-match-2015\",\"vienna-carlsen-simul-2015\",\"baku-world-cup-2015\",\"lev-gutman-2015\",\"vasteras-open-2015\",\"lev-gutman-rapid-2015\",\"5th-women-grandmaster-2015\",\"fide-world-junior-championship-open-2015\",\"fide-world-junior-championship-girls-2015\",\"frauen-schnellschach-em-2015\",\"pyramiden-cup-2015\",\"moscow-blitz-championship-2015\",\"sinquefield-Cup-UltimateMoves-2015\",\"sinquefield-2015\",\"spanish-championship-2015\",\"abudhabi2015\",\"open-sants-2015\",\"wgpiranmatch2015\",\"zmdi-open-2015\",\"uka-german-masters-2015\",\"ragger-mamedyarov-2015\",\"spanish-teams-honour-division-2015\",\"spanish-women-2015\",\"leko-li-2015\",\"french-wchampionship-2015\",\"french-championship-2015\",\"russian-women-championship-2015\",\"russian-championship-2015\",\"rtu-open-2015\",\"turkish-league-2015\",\"spanish-team-championship-2015\",\"arad-open-2015\",\"asian-championship-women-2015\",\"asian-championship-2015\",\"vellmarer-schachtage-2015\",\"hz-tournament-2015\",\"british-championship-2015\",\"china-russia-challenger-match-part-1-2015\",\"politiken-cup-2015\",\"biel-festival-2015\",\"andorra-open-2015\",\"sanmarti2015\",\"lake-sevan-2015\",\"china-russia-men-2015\",\"tsogo-sun-2015\",\"gelfand-ding-2015\",\"us-junior-closed-championship-2015\",\"dutch-championship-2015\",\"danzhou-super-gm-2015\",\"dortmund-2015\",\"russian-higher-league-2015\",\"edmonton-international-2015\",\"capablanca-memorial-2015-elite\",\"norway-chess-2015\",\"cez-chess-trophy-2015\",\"norway-chess-2015-blitz\",\"porto-mannu-open-2015\",\"french-top12-team-championship-2015\",\"european-womens-rapid-championship-2015\",\"arab-elite-championship-2015\",\"european-womens-championship-2015\",\"fide-grand-prix-4\",\"limburg-open-2015\",\"6th-rabat-blitz-marathon\",\"entercard-scandinavian-masters-2015\",\"nakhchivan-open-2015\",\"neoclassical-chess-madrid-2015\",\"russian-team-championship-2015\",\"italien-team-championship-2015\",\"world-team-championship-2015\",\"world-team-championship-women-2015\",\"battle-Of-Legends-Kasparov-Short-2015\",\"shamkir-gashimov-memorial-2015\",\"bangkok-open2015\",\"dubai-open2015\",\"us-championship-2015\",\"us-women-championship-2015\",\"schachbundesliga20142015\",\"neckarOpen2015\",\"philadelphia-open-2015\",\"fide-womens-world-championship-2015\",\"aeroflot-open-moscow-2015\",\"polish-championship-2015\",\"polish-women-championship-2015\",\"czech-tht-extraliga-2014-2015\",\"hawaii-chess-festival-2015\",\"reykjavikopen2015\",\"eicc2015\",\"grandPrixTbilisi2015\",\"zurich2015\",\"kortchnoiUhlmann2015\",\"zurich2015Blitz\",\"grenkechessclassic2015\",\"moscowOpen2015\",\"gibraltarChessCongress2015\",\"tataSteelMasters2015\",\"tataSteelChallengers2015\",\"aserbaijanChampionship2015\",\"australianOpen2014\",\"londonChess2014\",\"londonBlitz2014\",\"londonRapid2014\",\"russianWomenSuperfinal2014\",\"russianSuperfinal2014\",\"qatarMasters2014\",\"magistralBarcelona2014\",\"aronianNakamura2014\",\"worldChampionship2014\",\"talMemorial2014\",\"petrosianMemorial2014\",\"grandPrixTashkent2014\",\"univeChess2014Open\",\"univeChess2014Matches\",\"grandPrixBaku2014\",\"milionaireChess2014U2500KO\",\"milionaireChess2014U2350KO\",\"milionaireChess2014U2200KO\",\"milionaireChess2014U2000KO\",\"milionaireChess2014U1800KO\",\"milionaireChess2014U1600KO\",\"milionaireChess2014U1400KO\",\"milionaireChess2014U1200KO\",\"milionaireChess2014OpenKO\",\"milionaireChess2014Qualification\",\"milionaireChess2014Under\",\"milionaireChess2014Open\",\"bakuOpen2014\",\"bilbaomasters2014\",\"grenkechessclassic2014\",\"sinquefieldCup2014UltimateMoves\",\"sinquefieldCup2014\",\"olympiad2014women\",\"olympiad2014open\",\"biel2014\",\"dortmund2014\",\"norwayChess2014\",\"norwayChess2014Blitz\",\"usChamp2014Women\",\"usChamp2014\",\"sigeman2014\",\"nakhchivan-open-2014\",\"schachbundesliga20132014\",\"gashimovMemorial2014a\",\"gashimovMemorial2014b\",\"neckarOpen2014\",\"candidates2014\",\"zurich2014Classical\",\"zurich2014Blitz\",\"tataSteel2014b\",\"tataSteel2014a\",\"worldChampionship2013\",\"worldCup2013\",\"misc-2017\",\"swedish-championship-2017-random-games\",\"chinese-league-games-2016\",\"china-test-2016\",\"avro-1938\",\"keres-games\"]]";
    QVariantList tournaments = Chess24Messages::tournamentNamesFromJSON(dat);
    for(auto t:tournaments){
        this->tm->addTournament(t.toString());
    }
    return;*/

    /*connect(tm,&QAbstractItemModel::rowsAboutToBeInserted,[](const QModelIndex &mi,int first, int last){
        qDebug() << "rowsAboutToBeInserted: " << first << " " << last;
    });
    connect(tm,&QAbstractItemModel::rowsInserted,[](const QModelIndex &mi,int first, int last){
        qDebug() << "rowsInserted: " << first << " " << last;
    });

    */
    return;


    DiskNetworkCookieJar::loadCookieJar(jar);
    qnam.setCookieJar(&jar);
    c24.downloadUserData();
    connect(&c24ws,&Chess24Websocket::connected,[this](){
        WSRequest *req = c24ws.getTournamentIds();
        connect(req,&WSRequest::finished,[this](QString data){
            qDebug() << "Got tournament list";
            QVariantList tournaments = Chess24Messages::tournamentNamesFromJSON(data);
            this->tm->addTournaments(tournaments);
            /*for(int i=0;i<tournaments.size();++i){
                this->tm->addTournament(tournaments.at(i).toString(),i);
            }*/
        });
    });


    connect(&c24,&Chess24::loginResult,this,&Backend::loginResult);
    connect(&prepc24,&PrepareChess24WS::failure,[](QString error){
       qDebug() << "Connecting to WS failed";
       qDebug() << error;
    });

    connect(&prepc24,&PrepareChess24WS::success,this,[this](QString username, QString password){
       if(c24ws.isConnected()){
           return;
       }
       c24ws.connectWS(data,username,password);
    });
}

void Backend::setLoggedIn(bool loggedIn)
{
    m_loggedIn = loggedIn;
    emit loggedInChanged();
}

bool Backend::loggedIn()
{
    return m_loggedIn;
}

QString Backend::username()
{
    return m_username;
}

QString Backend::password()
{
    return m_password;
}

void Backend::setUsername(QString username)
{
    m_username = username;
    emit usernameChanged();
}

void Backend::setPassword(QString)
{
    m_password = password();
    emit passwordChanged();
}

void Backend::httpFinished()
{
    std::cout << "Got data" << std::endl;
    QByteArray data = reply->readAll();
    QString txt(data);

    pgn::GameCollection games;

    std::stringstream ss;
    ss << txt.toStdString();
    ss >> games;

    if(games.size()>0){
        pgn::Game g = games[0];
        std::cout << g.moves() << std::endl;
    }
}

void Backend::loginResult(UserData data)
{
    this->data = data;
    if(data.isRegistered){
        qDebug() << "Logged in";
        setLoggedIn(true);
        if(data.loginSource == UserData::LoginSource::USERPASS){
            DiskNetworkCookieJar *jar = static_cast<DiskNetworkCookieJar*>(qnam.cookieJar());
            jar->saveCookieJar();
        }
        prepc24.start();
    }else{
        qDebug() << "Login failed";
        setLoggedIn(false);
    }
}

void Backend::login(QString username, QString password)
{
    //Try with username and password if cookie didnt work
    c24.login(username,password);
}

void Backend::loginAccepted()
{
    qDebug() << "Dialog accepted";
}
