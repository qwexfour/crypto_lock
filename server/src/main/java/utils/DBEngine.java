package utils;

import base.FfEntity;
import dao.FfDAO;
import org.hibernate.HibernateException;
import org.hibernate.Session;

import java.util.ArrayList;
import java.util.List;

public class DBEngine {

    public List<String> parser(String message){
        List<String> data = new ArrayList<String>();
        String splitMessage[] =  message.split("#");
        for (int i = 0;i<splitMessage.length;i++){
           // System.out.println(splitMessage[i]);
            data.add( splitMessage[i]);
        }
        return data;
    }



    public String registration(List<String> data, boolean flag){
        String response;
        FfEntity ffEntity = new FfEntity();
        FfDAO ffDAO = new FfDAO();
        //userEntity.setId(1);
        try {
            ffEntity.setSurname(data.get(1));
            ffEntity.setFirstName(data.get(2));
            ffEntity.setPatronymic(data.get(3));
            ffEntity.setPublicKey(data.get(4)+"#"+data.get(5));
            if(flag){
                ffEntity.setAuth("true");
            }
            else {
                ffEntity.setAuth("false");
            }

            Session session = HiberSF.getSessionFactory().getCurrentSession();
            ffDAO.save(ffEntity, session);
            // session.getTransaction().commit();
        }catch (HibernateException e){
            return "m#ERROR\n";
        }
        response = "m#Success\n";
        return response;
    }
}
