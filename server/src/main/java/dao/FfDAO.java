package dao;

import base.FfEntity;
import intdao.FfInt;
import org.hibernate.Session;

public class FfDAO implements FfInt{
    @Override
    public void save(FfEntity ffEntity, Session session) {
        session.beginTransaction();
        session.save(ffEntity);
        session.getTransaction().commit();
    }
    @Override
    public void delete(FfEntity ffEntity, Session session) {
        session.beginTransaction();
        session.delete(ffEntity);
        session.getTransaction().commit();

    }
    @Override
    public void update(FfEntity ffEntity, Session session) {
        session.beginTransaction();
        session.update(ffEntity);
        session.getTransaction().commit();

    }
    @Override
    public FfEntity getEntityById(int id, Session session) {
        FfEntity ffEntity;
        session.beginTransaction();
        ffEntity = (FfEntity) session.get(FfEntity.class, 0);
        session.getTransaction().commit();
        return ffEntity;
    }
}
