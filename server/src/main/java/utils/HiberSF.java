package utils;

import org.hibernate.SessionFactory;
import org.hibernate.cfg.AnnotationConfiguration;
/**
 * Created by demon on 15.11.2016.
 */
public class HiberSF {

    private static SessionFactory sessionFactory = getSessionFactory();
    public static SessionFactory getSessionFactory() {
        try {
            if(sessionFactory == null) {
                // Create the SessionFactory from hibernate.cfg.xml
                sessionFactory = new AnnotationConfiguration().configure().buildSessionFactory();
            }
            return sessionFactory;
        } catch (Throwable ex) {
            // Make sure you log the exception, as it might be swallowed
            System.err.println("Initial SessionFactory creation failed." + ex);
            throw new ExceptionInInitializerError(ex);
        }

    }
    public static void killSF(SessionFactory sessionFactory){
        sessionFactory.close();
    }
}

