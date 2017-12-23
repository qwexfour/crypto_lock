package utils;

import base.FfEntity;

import java.util.List;
import java.util.Scanner;

public class Processor {
    public String rawMessage;
    public List<String> parsedMessage;
    public DBEngine dbEngine;
    private static final int ACTION = 0;
    private static final int SURNAME = 1;
    private static final int FIRSTNAME = 2;
    private static final int PATRONYMIC = 3;
    private static final int KEY = 0;
    private static final int GRANTS = 0;
    private static final String REGISTRATION_ACTION = "r";
    private static final String AUTHENTICATION_ACTION = "a";

    public String response;

    public Processor(String message){
        this.rawMessage = message;
        dbEngine = new DBEngine();
    }

    public void preParsing(){
      parsedMessage = dbEngine.parser(rawMessage);
    }

    public String start(){
        response = switchAction();
        return response;
    }

    public String switchAction(){
        if(parsedMessage.get(ACTION).equalsIgnoreCase(REGISTRATION_ACTION)){
            response = preRegistration();
        }
        if(parsedMessage.get(ACTION).equalsIgnoreCase(AUTHENTICATION_ACTION)){
            response = preAuthentication();
        }
        return response;
    }

    public String preRegistration(){
        SearchEngine searchEngine = new SearchEngine();
        List<FfEntity> ffEntities = searchEngine.getUserByloginAndPass(parsedMessage.get(SURNAME), parsedMessage.get(FIRSTNAME), parsedMessage.get(PATRONYMIC));


        if(ffEntities.size()==0){
            boolean regGrants = adminFactor();
            if(regGrants == true){
                response = dbEngine.registration(parsedMessage, regGrants);
            }
            if(regGrants == false){
                dbEngine.registration(parsedMessage, regGrants);
                response = "m#denied registration\n";
            }
        }
        else {
            response = "m#This user already exists in DB\n";
        }

        return  response;
    }

    public boolean adminFactor(){
        System.out.println("User:" + parsedMessage.get(SURNAME) + " " + parsedMessage.get(FIRSTNAME) + " " + parsedMessage.get(PATRONYMIC) + " wishes to register\n" +
                "Do you give the user the right to register? (yes/no)");
        Scanner in = new Scanner(System.in);
        while (true) {
            String adminResponse = in.nextLine();
            if ("yes".equalsIgnoreCase(adminResponse) || "ye".equalsIgnoreCase(adminResponse) || "y".equalsIgnoreCase(adminResponse)) {
                return true;
            }
            else if("n".equalsIgnoreCase(adminResponse)||"no".equalsIgnoreCase(adminResponse)){
                return false;
            }
            else {
                System.out.println("Incorrect input. Please try again");
            }
        }
    }

    public String  preAuthentication(){
        SearchEngine searchEngine = new SearchEngine();
        List<FfEntity> ffEntity = searchEngine.getUserByloginAndPass(parsedMessage.get(SURNAME), parsedMessage.get(FIRSTNAME), parsedMessage.get(PATRONYMIC));

        if(ffEntity.isEmpty()){
            response = "m#No such user in DB\n";
        }
        else {
            if("true".equalsIgnoreCase(ffEntity.get(GRANTS).getAuth())) {
                response = "k#" + ffEntity.get(KEY).getPublicKey() + "\n";
            }
            if("false".equalsIgnoreCase(ffEntity.get(KEY).getAuth())){
                response = "m#You have no grants\n";
            }
        }
        return response;
    }



}
