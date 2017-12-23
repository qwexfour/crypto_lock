package utils;

import base.FfEntity;
import org.hibernate.Session;

import java.util.List;

public class SearchEngine {


//    public UsersEntity getUserByloginAndPass(String surname, String firstname, String patronymic, Session session) {
//        UsersEntity user = null;
//        // SessionFactory sessionFactory = HiberSF.getSessionFactory();
//        // Session session = sessionFactory.openSession();
//        Query query = session.createQuery("from FfEntity where (surname = :surname) " +
//                "and (firstName = :firstname) and (patronymic =: patronymic)")
//                .setString("surname",surname)
//                .setString("firstname", firstname)
//                .setString("patronymic",patronymic);
//        user = (UsersEntity) query.uniqueResult();
//        return user;
//    }


    public List<FfEntity> getUserByloginAndPass(String surname, String firstname, String patronymic) {
        List<FfEntity> ffEntities = null;

        Session session = HiberSF.getSessionFactory().getCurrentSession();
        session.beginTransaction();
        ffEntities = (List<FfEntity>)
                session.createSQLQuery("select f.ID, f.surname, f.first_name, f.patronymic, f.public_key, f.auth from ff f " +
                        "WHERE " +
                        "f.surname = (:surname) AND " +
                        "f.first_name = (:firstname) AND " +
                        "f.patronymic = (:patronymic)")
                        .addEntity(FfEntity.class)
                        .setString("surname", surname)
                        .setString("firstname", firstname)
                        .setString("patronymic", patronymic)
                        .setMaxResults(1)
                        .list();
        session.getTransaction().commit();
        return  ffEntities;
    }



}
